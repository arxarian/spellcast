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

bool SocketCommunication::connectionLost() const
{
    return m_connectionLost;
}

bool SocketCommunication::connecting() const
{
    return m_connecting;
}

void SocketCommunication::connect(QString address, quint16 port)
{
    m_connection.m_address = QHostAddress(address);
    m_connection.m_port = port;

    socket.connectToHost(address, port);
}

void SocketCommunication::disconnect()
{
    if (socket.isValid())
    {
        socket.disconnectFromHost();
    }
}

void SocketCommunication::reconnect()
{
    m_reconnect = true;

    connect(m_connection.m_address.toString(), m_connection.m_port);
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

    QJsonObject jObject = d.object();

    const QString& messageType = jObject.value(QLatin1String("type")).toString();

    if (messageType == QLatin1String("hello"))
    {
        if (m_reconnect)
        {
            rejoinSession();
        }
        else
        {
            m_connection.m_sessionId = jObject.value(QLatin1String("sessionId")).toString();
        }
    }

    emit messageReceived(jObject);
}

void SocketCommunication::setState(QAbstractSocket::SocketState state)
{
    switch (state)
    {
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        m_connecting = true;

        emit connectingChanged();

        break;
    case QAbstractSocket::ConnectedState:
        m_connected = true;
        m_connecting = false;

        m_connectionLost = false;

        emit connectedChanged();
        emit connectingChanged();
        emit connectionLostChanged();

        break;
    case QAbstractSocket::UnconnectedState:
        m_connected = false;
        m_connectionLost = true;

        emit connectedChanged();
        emit connectionLostChanged();

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

void SocketCommunication::rejoinSession()
{
    QVariantMap map;
    map.insert("type", "rejoinSession");
    map.insert("sessionId", m_connection.m_sessionId);

    QJsonDocument json = QJsonDocument::fromVariant(map);

    sendMessage(json);
}
