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
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        sendResponse(socket, "error", false, "Format JSON invalide");
        return;
    }

    handleMessage(socket, doc.object());
}

void Server::handleMessage(QTcpSocket* socket, const QJsonObject& json) {
    QString type = json["type"].toString();

    if (type == "login") {
        handleLogin(socket, json["username"].toString(), json["password"].toString());
    }
    else if (type == "register") {
        handleRegister(socket, json["username"].toString(), json["password"].toString());
    }
    else if (type == "message") {
        // Verifier que le client est authentifie
        if (!m_clientUsernames.contains(socket)) {
            sendResponse(socket, "error", false, "Non authentifie");
            return;
        }

        QString sender = m_clientUsernames[socket];
        QString content = json["content"].toString();

        if (!content.isEmpty()) {
            qDebug() << "Message de" << sender << ":" << content;
            m_db.saveMessage(sender, content);
            broadcastMessage(sender, content, socket);
        }
    }
    else {
        sendResponse(socket, "error", false, "Type de message inconnu");
    }
}

void Server::handleLogin(QTcpSocket* socket, const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        sendResponse(socket, "login", false, "Nom d'utilisateur ou mot de passe vide");
        return;
    }

    if (m_db.authenticateUser(username, password)) {
        m_clientUsernames[socket] = username;
        m_db.setUserOnline(username, true);
        sendResponse(socket, "login", true, "Connexion reussie");
        qDebug() << "Utilisateur connecte :" << username;
    } else {
        sendResponse(socket, "login", false, "Identifiants incorrects");
    }
}

void Server::handleRegister(QTcpSocket* socket, const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        sendResponse(socket, "register", false, "Nom d'utilisateur ou mot de passe vide");
        return;
    }

    if (m_db.registerUser(username, password)) {
        sendResponse(socket, "register", true, "Inscription reussie");
        qDebug() << "Nouvel utilisateur enregistre :" << username;
    } else {
        sendResponse(socket, "register", false, "Nom d'utilisateur deja pris");
    }
}

void Server::sendResponse(QTcpSocket* socket, const QString& type, bool success, const QString& message) {
    QJsonObject response;
    response["type"] = type;
    response["success"] = success;
    response["message"] = message;

    QJsonDocument doc(response);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
}

void Server::broadcastMessage(const QString& sender, const QString& message, QTcpSocket* senderSocket) {
    QJsonObject json;
    json["type"] = "message";
    json["sender"] = sender;
    json["content"] = message;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";

    for (QTcpSocket *socket : m_clients) {
        if (socket != senderSocket && m_clientUsernames.contains(socket)) {
            socket->write(data);
        }
    }
}

void Server::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    // Mettre le statut hors ligne si l'utilisateur etait authentifie
    if (m_clientUsernames.contains(socket)) {
        QString username = m_clientUsernames[socket];
        m_db.setUserOnline(username, false);
        m_clientUsernames.remove(socket);
        qDebug() << "Utilisateur deconnecte :" << username;
    }

    qDebug() << "Client deconnecte :" << socket->peerAddress().toString();
    m_clients.removeAll(socket);
    socket->deleteLater();
}
