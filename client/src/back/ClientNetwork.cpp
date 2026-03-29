#include "ClientNetwork.hpp"
#include <QDebug>

ClientNetwork::ClientNetwork(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_lastPort(0)
    , m_loggedIn(false)
{
    connect(m_socket, &QTcpSocket::connected, this, &ClientNetwork::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientNetwork::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientNetwork::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &ClientNetwork::onError);
}

ClientNetwork::~ClientNetwork()
{
    disconnect();
}

void ClientNetwork::connectToServer(const QString &host, quint16 port)
{
    if (m_socket->state() == QAbstractSocket::ConnectingState || m_socket->state() == QAbstractSocket::HostLookupState) {
        emit error("Une tentative de connexion est deja en cours. Patiente quelques secondes avant de reessayer.");
        return;
    }

    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        emit error("Le client est deja connecte a un serveur. Deconnecte-toi avant de changer de serveur.");
        return;
    }

    m_lastHost = host.trimmed();
    m_lastPort = port;
    m_buffer.clear();
    m_loggedIn = false;
    m_socket->connectToHost(host, port);
}

void ClientNetwork::disconnect()
{
    m_socket->disconnectFromHost();
}

void ClientNetwork::sendMessage(const Message &message)
{
    QByteArray data = message.serialize();
    m_socket->write(data);
}

void ClientNetwork::sendLogin(const QString &username, const QString &password)
{
    m_username = username;
    // Pour compatibilité, on concatène username et password avec un séparateur spécial (ex: '\n')
    QString content = username + "\n" + password;
    Message msg(MessageType::LoginRequest, content);
    sendMessage(msg);
}

void ClientNetwork::sendChatMessage(const QString &content)
{
    if (!m_loggedIn) {
        emit error("Tu dois etre connecte avant d'envoyer un message.");
        return;
    }
    Message msg(MessageType::ChatMessage, content);
    msg.setSender(m_username);
    sendMessage(msg);
}

void ClientNetwork::sendWizz(const QString &targetUsername)
{
    if (!m_loggedIn) {
        emit error("Tu dois etre connecte avant d'envoyer un Wizz.");
        return;
    }
    Message msg(MessageType::Wizz, targetUsername);
    msg.setSender(m_username);
    sendMessage(msg);
}

bool ClientNetwork::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

bool ClientNetwork::isConnectionInProgress() const
{
    return m_socket->state() == QAbstractSocket::ConnectingState || m_socket->state() == QAbstractSocket::HostLookupState;
}

void ClientNetwork::onConnected()
{
    qDebug() << "Connected to server";
    emit connected();
}

void ClientNetwork::onDisconnected()
{
    qDebug() << "Disconnected from server";
    m_loggedIn = false;
    m_buffer.clear();
    emit disconnected();
}

void ClientNetwork::onReadyRead()
{
    QByteArray newData = m_socket->readAll();
    m_buffer.append(newData);

    // Process complete messages
    while (readMessage(m_buffer)) {
        // Message extracted and processed in readMessage
    }
}

void ClientNetwork::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
    emit error(friendlySocketError(socketError));
}

bool ClientNetwork::readMessage(QByteArray &buffer)
{
    // Need at least 10 bytes for header (magic 4 + version 1 + type 1 + length 4)
    if (buffer.size() < 10) {
        return false;
    }

    // Read length field (at offset 6)
    quint32 length = 0;
    length = (static_cast<quint8>(buffer[6]) << 24) |
             (static_cast<quint8>(buffer[7]) << 16) |
             (static_cast<quint8>(buffer[8]) << 8) |
             static_cast<quint8>(buffer[9]);

    // Check if we have the full message
    if (buffer.size() < 10 + length) {
        return false;
    }

    // Extract the message
    QByteArray messageData = buffer.left(10 + length);
    buffer.remove(0, 10 + length);

    // Process the message
    processData(messageData);
    return true;
}

void ClientNetwork::processData(const QByteArray &data)
{
    auto msgOpt = Message::deserialize(data);
    if (!msgOpt) {
        qDebug() << "Failed to deserialize message";
        return;
    }

    Message msg = *msgOpt;

    switch (msg.type()) {
    case MessageType::LoginResponse:
        if (msg.content() == "success") {
            m_loggedIn = true;
            emit loginSuccess(m_username);
        } else if (!m_loggedIn) {
            emit loginFailed(msg.content());
        } else {
            QStringList users;
            if (!msg.content().trimmed().isEmpty()) {
                users = msg.content().split(",", Qt::SkipEmptyParts);
                for (QString &user : users) {
                    user = user.trimmed();
                }
            }
            emit userListReceived(users);
        }
        break;

    case MessageType::ChatBroadcast:
    case MessageType::ChatMessage:
        emit messageReceived(msg);
        break;

    case MessageType::UserJoin:
        emit userJoined(msg.content().isEmpty() ? msg.sender() : msg.content());
        break;

    case MessageType::UserLeave:
        emit userLeft(msg.content().isEmpty() ? msg.sender() : msg.content());
        break;

    case MessageType::Wizz:
        emit wizzReceived(msg.sender());
        break;

    default:
        qDebug() << "Unknown message type:" << static_cast<int>(msg.type());
    }
}

QString ClientNetwork::friendlySocketError(QAbstractSocket::SocketError socketError) const
{
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        return "Connexion refusee par le serveur " + endpointDescription() + ". Verifie que WizzServer est lance et que le port est autorise dans le pare-feu.";
    case QAbstractSocket::RemoteHostClosedError:
        return m_loggedIn
            ? "Le serveur a ferme la connexion."
            : "Le serveur a coupe la connexion avant la fin de l'authentification.";
    case QAbstractSocket::HostNotFoundError:
        return "Serveur introuvable. Verifie l'adresse " + m_lastHost + ".";
    case QAbstractSocket::SocketAccessError:
        return "La connexion reseau a ete bloquee par Windows ou par le pare-feu.";
    case QAbstractSocket::SocketResourceError:
        return "Le client n'a pas pu reserver les ressources reseau necessaires.";
    case QAbstractSocket::NetworkError:
        return "Impossible d'atteindre le serveur " + endpointDescription() + ". Verifie que les deux PC sont sur le meme reseau.";
    case QAbstractSocket::AddressInUseError:
        return "Le port reseau utilise par le client est deja occupe localement.";
    case QAbstractSocket::SocketAddressNotAvailableError:
        return "L'adresse reseau locale n'est pas disponible sur ce PC.";
    case QAbstractSocket::UnsupportedSocketOperationError:
        return "Cette operation reseau n'est pas prise en charge sur ce systeme.";
    case QAbstractSocket::OperationError:
        return "Une autre operation reseau est deja en cours. Attends quelques secondes puis reessaie.";
    case QAbstractSocket::SslHandshakeFailedError:
        return "La negociation securisee a echoue.";
    case QAbstractSocket::TemporaryError:
        return "Erreur reseau temporaire. Reessaie dans quelques secondes.";
    case QAbstractSocket::UnknownSocketError:
        break;
    default:
        break;
    }

    const QString rawError = m_socket->errorString().trimmed();
    if (rawError.isEmpty()) {
        return "Erreur reseau inconnue pendant la connexion au serveur " + endpointDescription() + ".";
    }

    return "Erreur reseau avec le serveur " + endpointDescription() + " : " + rawError;
}

QString ClientNetwork::endpointDescription() const
{
    if (m_lastHost.isEmpty() || m_lastPort == 0) {
        return "selectionne";
    }

    return m_lastHost + ":" + QString::number(m_lastPort);
}
