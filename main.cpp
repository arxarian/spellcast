#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "spellcast.h"
#include "spellstats.h"
#include "spellprovider.h"

#include "socketcommunication.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<SpellStats>();
    qmlRegisterType<SpellCast>("SpellCast", 1, 0, "SpellCast");
    qmlRegisterType<SpellProvider>("SpellCast", 1, 0, "SpellProvider");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("spellcaster");
    QCoreApplication::setApplicationName("Spell Caster");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    SocketCommunication server;
    engine.rootContext()->setContextProperty("server", &server);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
