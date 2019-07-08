#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class SocketCommunication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    explicit SocketCommunication(QObject *parent = nullptr);

    Q_INVOKABLE void connect(QString ip, quint16 port);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendMessage(QJsonDocument jsonDoc);

    // messages
    Q_INVOKABLE void joinGame(QString userId);

    bool connected() const;

public slots:
    void receiveMessage();

private:
    QJsonObject CreateJsonObject(QVariantMap map);
    void setState(QAbstractSocket::SocketState state);

    QTcpSocket socket;

    bool m_connected = false;

signals:
    void connectedChanged();

    void MessageReceived();

};

#endif // SOCKETTEST_H
