#include "socketcommunication.h"

#include <QDebug>

SocketCommunication::SocketCommunication(QObject *parent) : QObject(parent)
{
    QObject::connect(&socket, &QTcpSocket::stateChanged, this, &SocketCommunication::setState);
    QObject::connect(&socket, &QTcpSocket::readyRead, this, &SocketCommunication::receiveMessage);
}

bool SocketCommunication::connected() const
{
    return m_connected;
}

void SocketCommunication::connect(QString ip, quint16 port)
{
    socket.connectToHost(ip, port);
}

void SocketCommunication::disconnect()
{
    if (socket.isValid())
    {
        socket.disconnectFromHost();
    }
}

void SocketCommunication::sendMessage(QJsonDocument jsonDoc)
{
    if (socket.isValid())
    {
        socket.write(jsonDoc.toJson());
    }
}

void SocketCommunication::receiveMessage()
{
    QByteArray arrData = socket.readAll();

    QJsonParseError error;
    QJsonDocument d = QJsonDocument::fromJson(arrData, &error);

    if (d.isNull())
    {
        qInfo() << error.errorString() << arrData;
    }

    emit messageReceived(d.object());
}

void SocketCommunication::setState(QAbstractSocket::SocketState state)
{
    switch (state)
    {
    case QAbstractSocket::ConnectedState:
        m_connected = true;
        emit connectedChanged();
        break;
    case QAbstractSocket::UnconnectedState:
        m_connected = false;
        emit connectedChanged();
        break;
    default:
        break;
    }
    qInfo() << "state" << state;
}

void SocketCommunication::setUserName(QString userId)
{
    m_userId = userId;
}

void SocketCommunication::sendSelectedSpells(QStringList spells)
{
    QVariantMap map;
    map.insert("type", "spellsSelected");
    map.insert("spells", spells);

    QJsonDocument json = QJsonDocument::fromVariant(map);

    sendMessage(json);
}

void SocketCommunication::sendSpellCast(QString id, qreal accuracy, qreal penalty, qint32 timeElapsedCompleted_ms, qint32 timeElapsedSpell_ms)
{
    QVariantMap map;
    map.insert("type", "spellCast");
    map.insert("spellId", id);
    map.insert("accuracy", accuracy);
    map.insert("penalty", penalty);
    map.insert("timeElapsedCompleted_ms", timeElapsedCompleted_ms);
    map.insert("timeElapsedSpell_ms", timeElapsedSpell_ms);

    QJsonDocument json = QJsonDocument::fromVariant(map);

    sendMessage(json);

}
