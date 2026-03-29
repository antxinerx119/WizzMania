#include "mainwindow.h"
#include "loginwindow.h"
#include "messagewindow.h"
#include "registerwindow.h"
#include "Message.h"

#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)
    , m_network(new ClientNetwork(this))
    , m_serverHost("127.0.0.1")
    , m_serverPort(12345)
    , m_wizzOffset(0, 0)
    , m_wizzAnimation(nullptr)
    , m_stack(new QStackedWidget)
    , m_loginWindow(new LoginWindow)
    , m_registerWindow(new RegisterWindow)
    , m_messagingWindow(new MessageWindow)
    , m_hasAuthenticatedSession(false)
    , m_registrationInProgress(false)
{
    m_loginWindow->setServerEndpoint(m_serverHost, m_serverPort);
    m_registerWindow->setServerEndpoint(m_serverHost, m_serverPort);

    m_stack->addWidget(m_loginWindow);
    m_stack->addWidget(m_registerWindow);
    m_stack->addWidget(m_messagingWindow);
    setCentralWidget(m_stack);

    connect(m_loginWindow, &LoginWindow::loginRequested, this, &MainWindow::onLoginRequested);
    connect(m_loginWindow, &LoginWindow::registerRequested, this, &MainWindow::onRegistrationPageRequested);
    connect(m_registerWindow, &RegisterWindow::registerRequested, this, &MainWindow::onRegisterRequested);
    connect(m_registerWindow, &RegisterWindow::backRequested, this, &MainWindow::onRegisterBackRequested);

    connect(m_network, &ClientNetwork::connected, this, &MainWindow::onConnected);
    connect(m_network, &ClientNetwork::disconnected, this, &MainWindow::onDisconnected);
    connect(m_network, &ClientNetwork::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(m_network, &ClientNetwork::loginFailed, this, &MainWindow::onLoginFailed);
    connect(m_network, &ClientNetwork::userListReceived, this, &MainWindow::onUserListReceived);
    connect(m_network, &ClientNetwork::userJoined, this, &MainWindow::onUserJoined);
    connect(m_network, &ClientNetwork::userLeft, this, &MainWindow::onUserLeft);
    connect(m_network, &ClientNetwork::messageReceived, this, &MainWindow::onMessageReceived);
    connect(m_network, &ClientNetwork::wizzReceived, this, &MainWindow::onWizzReceived);
    connect(m_network, &ClientNetwork::error, this, &MainWindow::onNetworkError);

    connect(m_messagingWindow, &MessageWindow::messageSent, this, [this](const QString &text) {
        m_network->sendChatMessage(text);
        m_messagingWindow->displayMessage(m_currentUsername, text);
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::onLoginRequested(const QString &username, const QString &password, const QString &host, quint16 port)
{
    startAuthentication(username, password, host, port, false);
}

void MainWindow::onRegistrationPageRequested(const QString &host, quint16 port)
{
    m_serverHost = host;
    m_serverPort = port;
    m_registerWindow->setServerEndpoint(m_serverHost, m_serverPort);
    m_stack->setCurrentIndex(1);
}

void MainWindow::onRegisterRequested(const QString &username, const QString &password, const QString &host, quint16 port)
{
    startAuthentication(username, password, host, port, true);
}

void MainWindow::onRegisterBackRequested(const QString &host, quint16 port)
{
    m_serverHost = host;
    m_serverPort = port;
    m_loginWindow->setServerEndpoint(m_serverHost, m_serverPort);
    m_stack->setCurrentIndex(0);
}

void MainWindow::startAuthentication(const QString &username, const QString &password, const QString &host, quint16 port, bool registrationFlow)
{
    const QString trimmedUsername = username.trimmed();
    const QString trimmedHost = host.trimmed();

    if (trimmedUsername.isEmpty()) {
        QMessageBox::warning(this, registrationFlow ? "Inscription" : "Connexion", "Le nom d'utilisateur est obligatoire.");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, registrationFlow ? "Inscription" : "Connexion", "Le mot de passe est obligatoire.");
        return;
    }

    if (trimmedHost.isEmpty()) {
        QMessageBox::warning(this, registrationFlow ? "Inscription" : "Connexion", "L'adresse du serveur est obligatoire.");
        return;
    }

    if (port == 0) {
        QMessageBox::warning(this, registrationFlow ? "Inscription" : "Connexion", "Le port du serveur est invalide.");
        return;
    }

    m_currentUsername = trimmedUsername;
    m_currentPassword = password;
    m_serverHost = trimmedHost;
    m_serverPort = port;
    m_registrationInProgress = registrationFlow;
    m_loginWindow->setServerEndpoint(m_serverHost, m_serverPort);
    m_registerWindow->setServerEndpoint(m_serverHost, m_serverPort);
    setAuthenticationUiBusy(true);
    m_network->connectToServer(m_serverHost, m_serverPort);
}

void MainWindow::onConnected()
{
    m_network->sendLogin(m_currentUsername, m_currentPassword);
    m_currentPassword.clear();
}

void MainWindow::onDisconnected()
{
    const bool showDisconnectWarning = m_hasAuthenticatedSession;
    m_hasAuthenticatedSession = false;
    m_registrationInProgress = false;
    setAuthenticationUiBusy(false);
    m_onlineUsers.clear();
    syncUserListDisplay();
    m_loginWindow->setServerEndpoint(m_serverHost, m_serverPort);
    m_registerWindow->setServerEndpoint(m_serverHost, m_serverPort);
    m_stack->setCurrentIndex(0);

    if (showDisconnectWarning) {
        QMessageBox::warning(this, "Connexion interrompue", "La connexion au serveur a ete interrompue.");
    }
}

void MainWindow::onLoginSuccess(const QString &username)
{
    m_hasAuthenticatedSession = true;
    m_registrationInProgress = false;
    setAuthenticationUiBusy(false);
    m_stack->setCurrentIndex(2);
    m_messagingWindow->setUsername(username);

    if (!m_onlineUsers.contains(username)) {
        m_onlineUsers.append(username);
        syncUserListDisplay();
    }
}

void MainWindow::onLoginFailed(const QString &error)
{
    setAuthenticationUiBusy(false);
    QMessageBox::warning(this, m_registrationInProgress ? "Inscription impossible" : "Connexion impossible", error);
    m_hasAuthenticatedSession = false;
    m_registrationInProgress = false;
    m_network->disconnect();
}

void MainWindow::onUserListReceived(const QStringList &users)
{
    m_onlineUsers = users;
    if (!m_currentUsername.isEmpty() && !m_onlineUsers.contains(m_currentUsername)) {
        m_onlineUsers.append(m_currentUsername);
    }
    syncUserListDisplay();
}

void MainWindow::onUserJoined(const QString &username)
{
    const QString trimmedUsername = username.trimmed();
    if (trimmedUsername.isEmpty()) {
        return;
    }

    if (!m_onlineUsers.contains(trimmedUsername)) {
        m_onlineUsers.append(trimmedUsername);
        syncUserListDisplay();
    }

    if (m_hasAuthenticatedSession && trimmedUsername != m_currentUsername) {
        m_messagingWindow->displayNotification(trimmedUsername + " a rejoint le chat.");
    }
}

void MainWindow::onUserLeft(const QString &username)
{
    const QString trimmedUsername = username.trimmed();
    if (trimmedUsername.isEmpty()) {
        return;
    }

    const bool removed = m_onlineUsers.removeAll(trimmedUsername) > 0;
    if (removed) {
        syncUserListDisplay();
    }

    if (m_hasAuthenticatedSession && trimmedUsername != m_currentUsername) {
        m_messagingWindow->displayNotification(trimmedUsername + " a quitte le chat.");
    }
}

void MainWindow::onMessageReceived(const Message &message)
{
    m_messagingWindow->displayMessage(message.sender(), message.content());
}

void MainWindow::onWizzReceived(const QString &fromUsername)
{
    m_messagingWindow->displayNotification(fromUsername + " vous a envoye un Wizz!");
    performWizzAnimation();
}

void MainWindow::onNetworkError(const QString &errorMessage)
{
    setAuthenticationUiBusy(false);
    QMessageBox::critical(this, m_hasAuthenticatedSession ? "Erreur reseau" : "Connexion impossible", errorMessage);
}

void MainWindow::setWizzOffset(const QPoint &offset)
{
    m_wizzOffset = offset;
    move(pos() + offset);
}

void MainWindow::performWizzAnimation()
{
    if (m_wizzAnimation) {
        delete m_wizzAnimation;
    }

    auto *group = new QSequentialAnimationGroup(this);

    for (int i = 0; i < 5; ++i) {
        auto *anim = new QPropertyAnimation(this, "wizzOffset", this);
        anim->setDuration(50);
        anim->setEndValue(i % 2 == 0 ? QPoint(-10, 0) : QPoint(10, 0));
        group->addAnimation(anim);
    }

    auto *finalAnim = new QPropertyAnimation(this, "wizzOffset", this);
    finalAnim->setDuration(50);
    finalAnim->setEndValue(QPoint(0, 0));
    group->addAnimation(finalAnim);

    m_wizzAnimation = group;
    connect(m_wizzAnimation, &QAbstractAnimation::finished, m_wizzAnimation, &QObject::deleteLater);
    m_wizzAnimation->start();
}

void MainWindow::syncUserListDisplay()
{
    QStringList sortedUsers = m_onlineUsers;
    sortedUsers.removeDuplicates();
    sortedUsers.sort(Qt::CaseInsensitive);
    m_messagingWindow->updateUserList(sortedUsers);
}

void MainWindow::setAuthenticationUiBusy(bool busy)
{
    m_loginWindow->setBusy(busy);
    m_registerWindow->setBusy(busy);
}
