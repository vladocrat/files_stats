#pragma once

#include <QObject>

#include "pimpl.h"

class FileStatsController final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged FINAL)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged FINAL)
public:

    static void registerType();
    static FileStatsController* instance();

    const std::vector<std::pair<QString, uint32_t>> stats() const;

    //! Property support
    qreal progress() const;
    bool isRunning() const;

signals:
    void isRunningChanged();
    void progressChanged();
    //! Property support

    void stateCleared();

public slots:
    void start(const QUrl& fileName);
    void stop();
    void pause();
    void resume();

private:
    DECLARE_PIMPL_CLASS(FileStatsController)

    FileStatsController();
    ~FileStatsController();

    void clear();
};

