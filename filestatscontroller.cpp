#include "filestatscontroller.h"

#include <QQmlEngine>
#include <QCoreApplication>

#include <unordered_map>

#include "worker.h"

namespace Constants
{
constexpr static const uint32_t STANDART_LINE_COUNT { 5 };
constexpr static const uint32_t DISPLAYED_ITEMS_COUNT { 15 };
} /// Constants

namespace Internal
{

auto findBiggestValues(const std::unordered_map<QString, uint32_t>& umap)
{
    std::vector<std::pair<QString, uint32_t>> vec(umap.begin(), umap.end());

    std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (vec.size() > Constants::DISPLAYED_ITEMS_COUNT) {
        vec.erase(vec.begin() + Constants::DISPLAYED_ITEMS_COUNT, vec.end());
    }

    return vec;
}

} /// Internal

struct FileStatsController::impl_t
{
    qreal progress { 0.0f };
    std::unordered_map<QString, uint32_t> stats;
    Worker worker;

    std::vector<std::pair<QString, uint32_t>> chart;
};

void FileStatsController::registerType()
{
    qmlRegisterSingletonInstance("FileStatsController", 1, 0, "FileStatsController", FileStatsController::instance());
}

FileStatsController::FileStatsController()
{
    createImpl();

    QObject::connect(qApp, &QCoreApplication::aboutToQuit, this, &FileStatsController::stop);
    QObject::connect(&impl().worker, &Worker::finished, this, &FileStatsController::isRunningChanged);

    QObject::connect(&impl().worker, &Worker::chunkProccessed, this, [this](uint32_t chunkSize, const QStringList& words) {
        Q_ASSERT(chunkSize != 0);
        Q_ASSERT(impl().worker.fileSize());

        for (const auto& word : words) {
            ++impl().stats[word];
        }

        //! Get the percentage to update progress bar
        //! Update graph
        impl().progress = chunkSize * 1.f / impl().worker.fileSize();
        emit progressChanged();
    }, Qt::QueuedConnection);
}

FileStatsController::~FileStatsController()
{
}

void FileStatsController::clear()
{
    impl().stats.clear();
    impl().progress = 0;

    emit stateCleared();
}

FileStatsController* FileStatsController::instance()
{
    static FileStatsController fsc;
    return &fsc;
}

void FileStatsController::start(const QUrl& fileName)
{
    if (impl().worker.isRunning()) {
        impl().worker.stop();
    }

    clear();

    impl().worker.run(fileName.toLocalFile(), Constants::STANDART_LINE_COUNT);
    emit isRunningChanged();
}

void FileStatsController::stop()
{
    impl().worker.stop();
}

void FileStatsController::pause()
{
    impl().worker.pause();
}

void FileStatsController::resume()
{
    impl().worker.resume();
}

bool FileStatsController::isRunning() const
{
    return impl().worker.isRunning();
}

qreal FileStatsController::progress() const
{
    return impl().progress;
}

const std::vector<std::pair<QString, uint32_t>> FileStatsController::stats() const
{
    return Internal::findBiggestValues(impl().stats);
}
