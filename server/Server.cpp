#include "Server.h"
#include <QDebug>
#include <QDataStream>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server()
{
    stop();
}

bool Server::start(quint16 port)
{
    if (!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "Failed to start server on port" << port;
        return false;
    }
    qDebug() << "Server started on port" << port;
    return true;
}

void Server::stop()
{
    m_server->close();
    // Disconnect all clients
    for (QTcpSocket *socket : m_clients.keys()) {
        socket->disconnectFromHost();
    }
    m_clients.clear();
    m_usernameToSocket.clear();
}

void Server::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *socket = m_server->nextPendingConnection();
        
        m_pendingLogins.insert(socket);
        
        connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
        connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
                this, &Server::onSocketError);
        
        qDebug() << "New client connected from" << socket->peerAddress().toString();
    }
}

void Server::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    m_clientReadBuffers[socket].append(data);

    // Process complete messages while available
    while (readMessageFromBuffer(socket)) {
        // Message extracted and processed in readMessageFromBuffer
    }
}

void Server::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString username = m_clients.value(socket);
    if (!username.isEmpty()) {
        m_db.setUserOnline(username, false);
        m_usernameToSocket.remove(username);
        
        // Notify others
        Message leaveMsg(MessageType::UserLeave, username);
        broadcast(leaveMsg, socket);
        
        emit clientDisconnected(username);
        qDebug() << "User" << username << "disconnected";
    }

    m_clients.remove(socket);
    m_pendingLogins.remove(socket);
    m_clientReadBuffers.remove(socket); // Clean up the buffer for this client
    
    qDebug() << "Client disconnected from" << socket->peerAddress().toString();
    socket->deleteLater();
}

void Server::onSocketError(QAbstractSocket::SocketError error)
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        qDebug() << "Socket error:" << socket->errorString();
        emit error(socket->errorString());
    }
}

void Server::processMessage(QTcpSocket *socket, const Message &msg)
{
    switch (msg.type()) {
    case MessageType::LoginRequest:
        handleLogin(socket, msg);
        break;
        
    case MessageType::ChatMessage:
        handleChatMessage(socket, msg);
        break;
        
    case MessageType::Wizz:
        handleWizz(socket, msg);
        break;
        
    default:
        qDebug() << "Unknown message type:" << static_cast<int>(msg.type());
    }
}

void Server::handleLogin(QTcpSocket *socket, const Message &msg)
{
    // On attend username et password concaténés avec '\n'
    QStringList parts = msg.content().split("\n");
    QString username = parts.value(0);
    QString password = parts.value(1);

    if (username.isEmpty()) {
        Message response(MessageType::LoginResponse, "Username cannot be empty");
        sendToClient(socket, response);
        return;
    }

    // Check if username is already taken
    if (m_usernameToSocket.contains(username)) {
        Message response(MessageType::LoginResponse, "Username already taken");
        sendToClient(socket, response);
        return;
    }

    // Authentification
    if (!m_db.authenticateUser(username, password)) {
        Message response(MessageType::LoginResponse, "Invalid username or password");
        sendToClient(socket, response);
        return;
    }

    // Add client
    m_clients[socket] = username;
    m_usernameToSocket[username] = socket;
    m_pendingLogins.remove(socket);

    // Send success response
    Message response(MessageType::LoginResponse, "success");
    sendToClient(socket, response);

    // Send user list
    sendUserList(socket);

    // Notify others
    Message joinMsg(MessageType::UserJoin, username);
    broadcast(joinMsg, socket);

    qDebug() << "User" << username << "logged in";
    emit clientConnected(username);
}

void Server::handleChatMessage(QTcpSocket *socket, const Message &msg)
{
    QString username = m_clients.value(socket);
    if (username.isEmpty()) {
        return;
    }

    QString content = msg.content();
    if (content.isEmpty()) {
        return;
    }

    // Save to database
    m_db.saveMessage(username, content);

    // Broadcast to all other clients
    Message broadcastMsg(MessageType::ChatBroadcast, content);
    broadcastMsg.setSender(username);
    broadcast(broadcastMsg, socket);

    qDebug() << "Message from" << username << ":" << content;
    emit messageReceived(username, content);
}

void Server::handleWizz(QTcpSocket *socket, const Message &msg)
{
    QString username = m_clients.value(socket);
    if (username.isEmpty()) {
        return;
    }

    QString target = msg.content();
    
    Message wizzMsg(MessageType::Wizz);
    wizzMsg.setSender(username);

    if (target.isEmpty()) {
        // Wizz everyone
        broadcast(wizzMsg, socket);
        qDebug() << "Wizz from" << username << "to everyone";
    } else {
        // Wizz specific user
        QTcpSocket *targetSocket = m_usernameToSocket.value(target);
        if (targetSocket) {
            sendToClient(targetSocket, wizzMsg);
            qDebug() << "Wizz from" << username << "to" << target;
        }
    }
}

void Server::sendToClient(QTcpSocket *socket, const Message &message)
{
    QByteArray data = message.serialize();
    socket->write(data);
}

void Server::broadcast(const Message &message, QTcpSocket *excludeSocket)
{
    QByteArray data = message.serialize();
    
    for (QTcpSocket *socket : m_clients.keys()) {
        if (socket != excludeSocket) {
            socket->write(data);
        }
    }
}

void Server::sendUserList(QTcpSocket *socket)
{
    QStringList users = m_clients.values();
    QString userList = users.join(",");
    
    Message msg(MessageType::LoginResponse, userList);
    sendToClient(socket, msg);
}

bool Server::readMessageFromBuffer(QTcpSocket* socket)
{
    if (!m_clientReadBuffers.contains(socket)) {
        return false;
    }

    QByteArray& buffer = m_clientReadBuffers[socket];

    // We need at least 10 bytes to read the header (magic: 4, version: 1, type: 1, length: 4)
    if (buffer.size() < 10) {
        return false;
    }

    // Peek at the length to check if we have the complete message
    quint32 messageLength;
    QDataStream stream(buffer);
    stream.setVersion(QDataStream::Qt_6_0);

    quint32 magic;
    quint8 version;
    quint8 type;
    stream >> magic >> version >> type >> messageLength;

    // Check if we have the complete message (header + payload)
    int totalSize = 10 + messageLength;
    if (buffer.size() < totalSize) {
        return false; // Wait for more data
    }

    // Extract and deserialize the message
    QByteArray messageData = buffer.mid(0, totalSize);
    buffer.remove(0, totalSize);

    auto msgOpt = Message::deserialize(messageData);
    if (!msgOpt) {
        qDebug() << "Failed to deserialize message";
        return true; // Consume the bad data and continue
    }

    Message msg = *msgOpt;
    processMessage(socket, msg);
    return true; // Successfully processed a message
}

