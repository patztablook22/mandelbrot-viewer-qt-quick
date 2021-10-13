#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "manager.h"
#include "renderer.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<Manager>("org.manager", 1, 0, "Manager");
    qmlRegisterType<Renderer>("org.renderer", 1, 0, "Renderer");
    engine.load(url);

    return app.exec();
}
