#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "spellcast.h"
#include "spellstats.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<SpellStats>();
    qmlRegisterType<SpellCast>("SpellCast", 1, 0, "SpellCast");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

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
