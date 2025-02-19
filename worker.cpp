#include "worker.h"

#include <QtConcurrent/QtConcurrent>
#include <QWaitCondition>

#include <atomic>

#include "fileparser.h"

struct Worker::impl_t
{
    std::atomic<bool> isRunning { false };
    std::atomic<bool> paused { false };

    FileParser parser;

    int fileSize { 0 };

    QWaitCondition pauseCond;
    QMutex locker;
};

Worker::Worker()
{
    createImpl();
}

Worker::~Worker()
{

}

bool Worker::isRunning() const
{
    return impl().isRunning;
}

bool Worker::isPaused() const
{
    return impl().paused;
}

void Worker::pause()
{
    if (impl().paused) {
        return;
    }

    impl().paused = true;
}

void Worker::resume()
{
    if (!impl().paused) {
        return;
    }

    impl().locker.lock();
    impl().paused = false;
    impl().pauseCond.wakeAll();
    impl().locker.unlock();
}

void Worker::run(const QString& fileName, uint32_t lineCount)
{
    if (impl().isRunning) {
        return;
    }

    if (impl().parser.initialized()) {
        impl().parser.clear();
    }

    if (!impl().parser.init(fileName, lineCount)) {
        qDebug() << "Worker: could not init parser.";
        emit finished();
        return;
    }

    impl().fileSize = impl().parser.fileSize();
    impl().isRunning = true;

    [[maybe_unused]] auto future = QtConcurrent::run([this]() {
        uint32_t bytesRead { 0 };

        do {
            impl().locker.lock();

            if (impl().paused) {
                impl().pauseCond.wait(&impl().locker);
            }

            impl().locker.unlock();

            if (!impl().isRunning) {
                impl().parser.clear();
                break;
            }

            const auto chunk = impl().parser.getChunk(bytesRead);
            emit chunkProccessed(bytesRead, chunk);
        } while (bytesRead != fileSize() && impl().isRunning);

        impl().isRunning = false;
        emit finished();
    });
}

void Worker::stop()
{
    impl().isRunning = false;

    impl().locker.lock();

    if (impl().paused) {
        impl().paused = false;
        impl().pauseCond.wakeAll();
    }

    impl().locker.unlock();
}

int Worker::fileSize() const
{
    return impl().fileSize;
}
