#include "rankwordchart.h"
#include "rankwordfile.h"
#include "rankwordfilecontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setOrganizationName("somename");
    app.setOrganizationDomain("somename");
    app.setApplicationName("Amazing Application");

    qmlRegisterType<RankWordChart>("UI", 1, 0, "RankWordChart");
    qmlRegisterType<RankWordFileController>("Controller", 1, 0, "RankWordFileController");
    qmlRegisterSingletonType(QUrl("qrc:/Styles.qml"), "Styles", 1, 0, "Styles");
    qmlRegisterSingletonType(QUrl("qrc:/MainState.qml"), "States", 1, 0, "MainState");
    QQuickStyle::setStyle("Material");

    RankWordFile file;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
