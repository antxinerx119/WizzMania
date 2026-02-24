#include "ClientNetwork.h"
#include <QDebug>

ClientNetwork::ClientNetwork(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_loggedIn(false)
{
    connect(m_socket, &QTcpSocket::connected, this, &ClientNetwork::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientNetwork::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientNetwork::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &ClientNetwork::onError);
}

ClientNetwork::~ClientNetwork()
{
    disconnect();
}

void ClientNetwork::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

void ClientNetwork::disconnect()
{
    m_socket->disconnectFromHost();
}

void ClientNetwork::sendMessage(const Message &message)
{
    QByteArray data = message.serialize();
    m_socket->write(data);
}

void ClientNetwork::sendLogin(const QString &username, const QString &password)
{
    m_username = username;
    // Pour compatibilité, on concatène username et password avec un séparateur spécial (ex: '\n')
    QString content = username + "\n" + password;
    Message msg(MessageType::LoginRequest, content);
    sendMessage(msg);
}

void ClientNetwork::sendChatMessage(const QString &content)
{
    if (!m_loggedIn) {
        emit error("Not logged in");
        return;
    }
    Message msg(MessageType::ChatMessage, content);
    msg.setSender(m_username);
    sendMessage(msg);
}

void ClientNetwork::sendWizz(const QString &targetUsername)
{
    if (!m_loggedIn) {
        emit error("Not logged in");
        return;
    }
    Message msg(MessageType::Wizz, targetUsername);
    msg.setSender(m_username);
    sendMessage(msg);
}

bool ClientNetwork::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void ClientNetwork::onConnected()
{
    qDebug() << "Connected to server";
    emit connected();
}

void ClientNetwork::onDisconnected()
{
    qDebug() << "Disconnected from server";
    m_loggedIn = false;
    emit disconnected();
}

void ClientNetwork::onReadyRead()
{
    QByteArray newData = m_socket->readAll();
    m_buffer.append(newData);

    // Process complete messages
    while (readMessage(m_buffer)) {
        // Message extracted and processed in readMessage
    }
}

void ClientNetwork::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
    emit error(m_socket->errorString());
}

bool ClientNetwork::readMessage(QByteArray &buffer)
{
    // Need at least 10 bytes for header (magic 4 + version 1 + type 1 + length 4)
    if (buffer.size() < 10) {
        return false;
    }

    // Read length field (at offset 6)
    quint32 length = 0;
    length = (static_cast<quint8>(buffer[6]) << 24) |
             (static_cast<quint8>(buffer[7]) << 16) |
             (static_cast<quint8>(buffer[8]) << 8) |
             static_cast<quint8>(buffer[9]);

    // Check if we have the full message
    if (buffer.size() < 10 + length) {
        return false;
    }

    // Extract the message
    QByteArray messageData = buffer.left(10 + length);
    buffer.remove(0, 10 + length);

    // Process the message
    processData(messageData);
    return true;
}

void ClientNetwork::processData(const QByteArray &data)
{
    auto msgOpt = Message::deserialize(data);
    if (!msgOpt) {
        qDebug() << "Failed to deserialize message";
        return;
    }

    Message msg = *msgOpt;

    switch (msg.type()) {
    case MessageType::LoginResponse:
        // Handle login response
        if (msg.content() == "success") {
            m_loggedIn = true;
            emit loginSuccess(m_username);
        } else {
            emit loginFailed(msg.content());
        }
        break;

    case MessageType::ChatBroadcast:
    case MessageType::ChatMessage:
        emit messageReceived(msg);
        break;

    case MessageType::UserJoin:
        emit userJoined(msg.sender());
        break;

    case MessageType::UserLeave:
        emit userLeft(msg.sender());
        break;

    case MessageType::Wizz:
        emit wizzReceived(msg.sender());
        break;

    default:
        qDebug() << "Unknown message type:" << static_cast<int>(msg.type());
    }
}
