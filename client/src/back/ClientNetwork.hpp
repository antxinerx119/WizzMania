#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <QObject>
#include <QStringList>
#include <QTcpSocket>
#include "Message.hpp"
#include "Protocol.hpp"

class ClientNetwork : public QObject
{
    Q_OBJECT

public:
    explicit ClientNetwork(QObject *parent = nullptr);
    ~ClientNetwork();

    void connectToServer(const QString &host, quint16 port);
    void disconnect();

    void sendMessage(const Message &message);
    void sendLogin(const QString &username, const QString &password = "");
    void sendChatMessage(const QString &content);

    bool isConnected() const;
    bool isConnectionInProgress() const;
    QString username() const { return m_username; }

signals:
    void connected();
    void disconnected();
    void loginSuccess(const QString &username);
    void loginFailed(const QString &error);
    void userListReceived(const QStringList &users);
    void messageReceived(const Message &message);
    void userJoined(const QString &username);
    void userLeft(const QString &username);
    void error(const QString &errorMessage);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QString friendlySocketError(QAbstractSocket::SocketError socketError) const;
    QString endpointDescription() const;
    void processData(const QByteArray &data);
    bool readMessage(QByteArray &buffer);

    QTcpSocket *m_socket;
    QString m_username;
    QString m_lastHost;
    quint16 m_lastPort;
    QByteArray m_buffer;
    bool m_loggedIn;
};

#endif // CLIENTNETWORK_H
