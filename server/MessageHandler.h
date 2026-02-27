#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QTcpSocket>
#include <QByteArray>

class Server;
class Message;

class MessageHandler {
public:
    explicit MessageHandler(Server *server);
    ~MessageHandler() = default;

    // Parses raw data and routes to the appropriate handler
    void processData(QTcpSocket *socket, const QByteArray &data);

private:
    void handleLogin(QTcpSocket *socket, const Message &msg);
    void handleChatMessage(QTcpSocket *socket, const Message &msg);
    void handleWizz(QTcpSocket *socket, const Message &msg);

    Server *m_server;
};

#endif // MESSAGEHANDLER_H
