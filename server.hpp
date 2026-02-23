#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include "database.hpp"

class Server : public QTcpServer {
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    bool startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void handleMessage(QTcpSocket* socket, const QJsonObject& json);
    void handleLogin(QTcpSocket* socket, const QString& username, const QString& password);
    void handleRegister(QTcpSocket* socket, const QString& username, const QString& password);
    void sendResponse(QTcpSocket* socket, const QString& type, bool success, const QString& message);
    void broadcastMessage(const QString& sender, const QString& message, QTcpSocket* senderSocket);

    QList<QTcpSocket*> m_clients;
    QMap<QTcpSocket*, QString> m_clientUsernames;
    Database m_db;
};

#endif // SERVER_HPP
