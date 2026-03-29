#include "MessageHandler.hpp"
#include "Server.hpp"
#include "Message.hpp"
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
        m_server->handleLogin(socket, msg);
        break;
        
    case MessageType::ChatMessage:
        handleChatMessage(socket, msg);
        break;

    default:
        qDebug() << "Unknown message type:" << static_cast<int>(msg.type());
    }
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
