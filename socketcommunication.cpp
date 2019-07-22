#include "socketcommunication.h"

#include <QDebug>
#include <QMetaEnum>

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
    m_buffer.append(socket.readAll());

    while (m_buffer.isEmpty() == false)
    {
        qint32 nTerminator = m_buffer.indexOf("\r\n");

        if (nTerminator < 0)
        {
            break;
        }

        QByteArray arrData = m_buffer.left(nTerminator + 2);
        m_buffer.remove(0, nTerminator + 2);

        QJsonParseError error;
        QJsonDocument d = QJsonDocument::fromJson(arrData, &error);

        if (d.isNull())
        {
            qInfo() << error.errorString() << arrData;
            emit log("json error:" + error.errorString());
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

                joinSession();
            }
        }

        emit messageReceived(jObject);
    }
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
        m_connectionLost = m_connected;
        m_connecting = false;
        m_connected = false;

        emit connectedChanged();
        emit connectingChanged();
        emit connectionLostChanged();

        break;
    default:
        break;
    }

    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    emit log(QString("connection: %1").arg(metaEnum.valueToKey(state)));

    qInfo() << "state" << state;
}

void SocketCommunication::setPlayerName(QString playerName)
{
    m_playerName = playerName;
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

void SocketCommunication::joinSession()
{
    QVariantMap map;
    map.insert("type", "joinSession");
    map.insert("name", m_playerName);

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
