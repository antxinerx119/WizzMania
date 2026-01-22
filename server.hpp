#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
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
    void broadcastMessage(const QString& message, QTcpSocket* sender);
    QList<QTcpSocket*> m_clients;
    Database m_db;
};

#endif // SERVER_HPP
