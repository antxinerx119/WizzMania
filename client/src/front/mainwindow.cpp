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
    , m_wizzOffset(0, 0)
    , m_wizzAnimation(nullptr)
    , m_stack(new QStackedWidget)
    , m_loginWindow(new LoginWindow)
    , m_registerWindow(new RegisterWindow)
    , m_messagingWindow(new MessageWindow)
    , m_hasAuthenticatedSession(false)
    , m_registrationInProgress(false)
{
    m_stack->addWidget(m_loginWindow);
    m_stack->addWidget(m_registerWindow);
    m_stack->addWidget(m_messagingWindow);
    setCentralWidget(m_stack);

    connect(m_loginWindow, &LoginWindow::loginRequested, this, &MainWindow::onLoginRequested);
    connect(m_loginWindow, &LoginWindow::registerRequested, this, [this]() {
        m_stack->setCurrentIndex(1);
    });
    connect(m_registerWindow, &RegisterWindow::registerRequested, this, &MainWindow::onRegisterRequested);
    connect(m_registerWindow, &RegisterWindow::backRequested, this, [this]() {
        m_stack->setCurrentIndex(0);
    });

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

void MainWindow::onLoginRequested(const QString &username, const QString &password)
{
    startAuthentication(username, password, false);
}

void MainWindow::onRegisterRequested(const QString &username, const QString &password)
{
    startAuthentication(username, password, true);
}

void MainWindow::startAuthentication(const QString &username, const QString &password, bool registrationFlow)
{
    const QString trimmedUsername = username.trimmed();

    if (trimmedUsername.isEmpty()) {
        QMessageBox::warning(this, registrationFlow ? "Inscription" : "Connexion", "Le nom d'utilisateur est obligatoire.");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, registrationFlow ? "Inscription" : "Connexion", "Le mot de passe est obligatoire.");
        return;
    }

    m_currentUsername = trimmedUsername;
    m_currentPassword = password;
    m_registrationInProgress = registrationFlow;
    m_network->connectToServer("127.0.0.1", 12345);
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
    m_onlineUsers.clear();
    syncUserListDisplay();
    m_stack->setCurrentIndex(0);

    if (showDisconnectWarning) {
        QMessageBox::warning(this, "Disconnected", "You have been disconnected from the server.");
    }
}

void MainWindow::onLoginSuccess(const QString &username)
{
    m_hasAuthenticatedSession = true;
    m_registrationInProgress = false;
    m_stack->setCurrentIndex(2);
    m_messagingWindow->setUsername(username);

    if (!m_onlineUsers.contains(username)) {
        m_onlineUsers.append(username);
        syncUserListDisplay();
    }
}

void MainWindow::onLoginFailed(const QString &error)
{
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
    QMessageBox::critical(this, "Network Error", errorMessage);
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
