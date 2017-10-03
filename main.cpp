#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <match3model.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Match3Model           match3model;

    engine.rootContext()->setContextProperty("match3Model", &match3model);
    engine.load(QUrl(QStringLiteral("../Match3Simple/main.qml")));

    return app.exec();
}
