#include "FilterWordModel.h"
#include "RankWordChart.h"
#include "RankWordFile.h"
#include "RankWordFileController.h"
#include "RegExpIns.h"

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
    qmlRegisterSingletonType<RankWordFileController>("Controller", 1, 0, "RankWordFileController", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        RankWordFileController* ins = new RankWordFileController();
        return ins;
    });
    qmlRegisterSingletonType<FilterWordModel>("Model", 1, 0, "FilterWordModel", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        FilterWordModel* ins = &FilterWordModel::getInstance();
        return ins;
    });
    qmlRegisterSingletonType(QUrl("qrc:/Styles.qml"), "Styles", 1, 0, "Styles");
    qmlRegisterSingletonType(QUrl("qrc:/MainState.qml"), "States", 1, 0, "MainState");
    qmlRegisterSingletonType<RegExpIns>("Types", 1, 0, "RegExpIns", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        RegExpIns *ins = new RegExpIns();
        return ins;
    });
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
