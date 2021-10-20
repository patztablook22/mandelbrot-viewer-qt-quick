#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "renderer.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setOrganizationName("mandelbrot");
    app.setOrganizationDomain("mandelbrot");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<Palette>("org.palette", 0, 0, "Palette");
    qmlRegisterType<Renderer>("org.renderer", 0, 0, "Renderer");
    engine.load(url);

    return app.exec();
}
