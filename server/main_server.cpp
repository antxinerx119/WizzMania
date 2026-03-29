#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "Server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("WizzMania Server");

    quint16 port = 12345;

    QStringList args = app.arguments();
    int portIndex = args.indexOf("--port");
    if (portIndex != -1 && portIndex + 1 < args.size()) {
        port = args.at(portIndex + 1).toUShort();
    }

    Server server;

    QObject::connect(&server, &Server::clientConnected, [](const QString &username) {
        qDebug() << "[SERVER] User connected:" << username;
    });

    QObject::connect(&server, &Server::clientDisconnected, [](const QString &username) {
        qDebug() << "[SERVER] User disconnected:" << username;
    });

    QObject::connect(&server, &Server::messageReceived, [](const QString &from, const QString &content) {
        qDebug() << "[SERVER] Message from" << from << ":" << content;
    });

    QObject::connect(&server, &Server::error, [](const QString &errorMessage) {
        qDebug() << "[SERVER] Error:" << errorMessage;
    });

    if (!server.start(port)) {
        std::cerr << "Failed to start server on port " << port << std::endl;
        return 1;
    }

    qDebug() << "[SERVER] WizzMania server running on port" << port;
    qDebug() << "[SERVER] Waiting for connections...";

    return app.exec();
}
