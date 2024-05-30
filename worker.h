#pragma once

#include <QObject>

#include "pimpl.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
    ~Worker();

    bool isRunning() const;
    bool isPaused() const;

    void pause();
    void resume();
    void run(const QString& fileName, uint32_t lineCount);
    void stop();

    int fileSize() const;

signals:
    //! const& doesn't matter, passed between threads, causes deep copy
    void chunkProccessed(uint32_t chunkSize, const QStringList& words);

    void wordProcessed(QString word);
    void chunkProcessed(int chunkSize);

    void finished();

private:
    DECLARE_PIMPL_CLASS(Worker)
};

