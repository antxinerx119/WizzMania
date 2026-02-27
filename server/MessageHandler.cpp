#include "MessageHandler.h"
#include "Server.h"
#include "Message.h"
#include <QDebug>

MessageHandler::MessageHandler(Server *server)
    : m_server(server)
{
}

void MessageHandler::processData(QTcpSocket *socket, const QByteArray &data)
{
    auto msgOpt = Message::deserialize(data);
    if (!msgOpt) {
        qDebug() << "Failed to deserialize message in MessageHandler";
        return;
    }
    const Message &msg = *msgOpt;

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

void MessageHandler::handleLogin(QTcpSocket *socket, const Message &msg)
{
    // On attend username et password concaténés avec '\n'
    QStringList parts = msg.content().split("\n");
    QString username = parts.value(0);
    QString password = parts.value(1);

    if (username.isEmpty()) {
        Message response(MessageType::LoginResponse, "Username cannot be empty");
        m_server->sendToClient(socket, response);
        return;
    }

    // Check if username is already taken
    if (m_server->m_usernameToSocket.contains(username)) {
        Message response(MessageType::LoginResponse, "Username already taken");
        m_server->sendToClient(socket, response);
        return;
    }

    // Authentification
    if (!m_server->m_db.authenticateUser(username, password)) {
        Message response(MessageType::LoginResponse, "Invalid username or password");
        m_server->sendToClient(socket, response);
        return;
    }

    // Add client
    m_server->m_clients[socket] = username;
    m_server->m_usernameToSocket[username] = socket;
    m_server->m_pendingLogins.remove(socket);

    // Send success response
    Message response(MessageType::LoginResponse, "success");
    m_server->sendToClient(socket, response);

    // Send user list
    m_server->sendUserList(socket);

    // Notify others
    Message joinMsg(MessageType::UserJoin, username);
    m_server->broadcast(joinMsg, socket);

    qDebug() << "User" << username << "logged in";
    emit m_server->clientConnected(username);
}

void MessageHandler::handleChatMessage(QTcpSocket *socket, const Message &msg)
{
    QString username = m_server->m_clients.value(socket);
    if (username.isEmpty()) {
        return;
    }

    QString content = msg.content();
    if (content.isEmpty()) {
        return;
    }

    // Save to database
    m_server->m_db.saveMessage(username, content);

    // Broadcast to all other clients
    Message broadcastMsg(MessageType::ChatBroadcast, content);
    broadcastMsg.setSender(username);
    m_server->broadcast(broadcastMsg, socket);

    qDebug() << "Message from" << username << ":" << content;
    emit m_server->messageReceived(username, content);
}

void MessageHandler::handleWizz(QTcpSocket *socket, const Message &msg)
{
    QString username = m_server->m_clients.value(socket);
    if (username.isEmpty()) {
        return;
    }

    QString target = msg.content();
    
    Message wizzMsg(MessageType::Wizz);
    wizzMsg.setSender(username);

    if (target.isEmpty()) {
        // Wizz everyone
        m_server->broadcast(wizzMsg, socket);
        qDebug() << "Wizz from" << username << "to everyone";
    } else {
        // Wizz specific user
        QTcpSocket *targetSocket = m_server->m_usernameToSocket.value(target);
        if (targetSocket) {
            m_server->sendToClient(targetSocket, wizzMsg);
            qDebug() << "Wizz from" << username << "to" << target;
        }
    }
}
