#include "server.hpp"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent) {}

bool Server::startServer(quint16 port) {
    if (!listen(QHostAddress::Any, port)) {
        qDebug() << "Le serveur n'a pas pu demarrer sur le port" << port;
        return false;
    }
    qDebug() << "Serveur demarre sur le port" << port;
    return true;
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        delete socket;
        return;
    }

    m_clients << socket;
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    qDebug() << "Nouveau client connecte :" << socket->peerAddress().toString();
}

void Server::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    if (!message.isEmpty()) {
        qDebug() << "Message recu :" << message;
        m_db.saveMessage(socket->peerAddress().toString(), message);
        broadcastMessage(message, socket);
    }
}

void Server::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "Client deconnecte :" << socket->peerAddress().toString();
    m_clients.removeAll(socket);
    socket->deleteLater();
}

void Server::broadcastMessage(const QString& message, QTcpSocket* senderSocket) {
    QByteArray data = message.toUtf8();
    for (QTcpSocket *socket : m_clients) {
        if (socket != senderSocket) {
            socket->write(data);
        }
    }
}
