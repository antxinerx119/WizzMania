#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include "database.hpp"
#include "Message.h"
#include "Protocol.h"

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool start(quint16 port);
    void stop();

signals:
    void clientConnected(const QString &username);
    void clientDisconnected(const QString &username);
    void messageReceived(const QString &from, const QString &content);
    void error(const QString &errorMessage);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    void processMessage(QTcpSocket *socket, const QByteArray &data);
    void handleLogin(QTcpSocket *socket, const Message &msg);
    void handleChatMessage(QTcpSocket *socket, const Message &msg);
    void handleWizz(QTcpSocket *socket, const Message &msg);

    void sendToClient(QTcpSocket *socket, const Message &message);
    void broadcast(const Message &message, QTcpSocket *excludeSocket = nullptr);
    void sendUserList(QTcpSocket *socket);
    bool readMessageFromBuffer(QTcpSocket* socket);

    QTcpServer *m_server;
    QMap<QTcpSocket*, QString> m_clients; // socket -> username
    QMap<QString, QTcpSocket*> m_usernameToSocket; // username -> socket
    Database m_db;
    QSet<QTcpSocket*> m_pendingLogins; // clients awaiting login
    QMap<QTcpSocket*, QByteArray> m_clientReadBuffers; // Buffer for incoming data for each client
};

#endif // SERVER_H
