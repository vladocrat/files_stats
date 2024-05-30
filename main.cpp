#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "filestatscontroller.h"
#include "graph.h"
#include <QFile>

namespace Internal
{

void registerTypes()
{
    FileStatsController::registerType();
    Graph::registerType();
}

} /// Internal

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Internal::registerTypes();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/file_stats/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
