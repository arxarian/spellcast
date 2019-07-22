#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>

class SocketCommunication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool connecting READ connecting NOTIFY connectingChanged)
    Q_PROPERTY(bool connectionLost READ connectionLost NOTIFY connectionLostChanged)

public:
    explicit SocketCommunication(QObject *parent = nullptr);

    struct Connection
    {
        QHostAddress m_address;
        quint16 m_port;
        QString m_sessionId;
    };

    Q_INVOKABLE void connect(QString address, quint16 port);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void reconnect();
    Q_INVOKABLE void setPlayerName(QString playerName);

    Q_INVOKABLE void sendMessage(QJsonDocument jsonDoc);

    // messages
    Q_INVOKABLE void sendSelectedSpells(QStringList spells);
    Q_INVOKABLE void sendSpellCast(QString id, qreal accuracy, qreal penalty,
                                   qint32 timeElapsedCompleted_ms, qint32 timeElapsedSpell_ms);

    void joinSession();
    void rejoinSession();
    bool connected() const;
    bool connecting() const;
    bool connectionLost() const;


public slots:
    void receiveMessage();

private:
    QJsonObject CreateJsonObject(QVariantMap map);
    void setState(QAbstractSocket::SocketState state);

    QTcpSocket socket;
    QString m_playerName;
    bool m_connected = false;
    bool m_connecting = false;
    bool m_connectionLost = false;
    bool m_reconnect = false;

    QByteArray m_buffer;

    Connection m_connection;

signals:
    void connectedChanged();
    void connectingChanged();
    void connectionLostChanged();
    void messageReceived(QJsonObject message);

    void log(QString log);
};

#endif // SOCKETTEST_H
