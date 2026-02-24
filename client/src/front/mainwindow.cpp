#include "mainwindow.h"
#include "loginwindow.h"
#include "messagewindow.h"
#include "src/common/Message.h"

#include <QStackedWidget>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_network(new ClientNetwork(this))
    , m_wizzOffset(0, 0)
    , m_wizzAnimation(nullptr)
{
    auto *stack = new QStackedWidget;

    auto *login = new LoginWindow;
    auto *messaging = new MessageWindow;

    stack->addWidget(login);      // index 0
    stack->addWidget(messaging);  // index 1

    setCentralWidget(stack);

    // Connect login signal to our handler
    connect(login, &LoginWindow::loginRequested, this, &MainWindow::onLoginRequested);

    // Connect network signals
    connect(m_network, &ClientNetwork::connected, this, &MainWindow::onConnected);
    connect(m_network, &ClientNetwork::disconnected, this, &MainWindow::onDisconnected);
    connect(m_network, &ClientNetwork::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(m_network, &ClientNetwork::loginFailed, this, &MainWindow::onLoginFailed);
    connect(m_network, &ClientNetwork::messageReceived, this, &MainWindow::onMessageReceived);
    connect(m_network, &ClientNetwork::wizzReceived, this, &MainWindow::onWizzReceived);
    connect(m_network, &ClientNetwork::error, this, &MainWindow::onNetworkError);

    // Store messaging window reference for later use (would need to make it accessible)
    // For now, we'll handle this differently
}

MainWindow::~MainWindow()
{
}

void MainWindow::onLoginRequested(const QString &username, const QString &password)
{
    m_currentUsername = username;
    m_currentPassword = password;
    // Connect to local server by default
    m_network->connectToServer("127.0.0.1", 12345);
}

void MainWindow::onConnected()
{
    m_network->sendLogin(m_currentUsername, m_currentPassword);
}

void MainWindow::onDisconnected()
{
    QMessageBox::warning(this, "Disconnected", "You have been disconnected from the server.");
}

void MainWindow::onLoginSuccess(const QString &username)
{
    // Switch to messaging window
    // Note: Need to access the messaging window - for now just show a message
    QMessageBox::information(this, "Connected", "Welcome " + username + "!");
}

void MainWindow::onLoginFailed(const QString &error)
{
    QMessageBox::warning(this, "Login Failed", error);
    m_network->disconnect();
}

void MainWindow::onMessageReceived(const Message &message)
{
    // Display message in the message window
    // This would need access to the MessageWindow
    qDebug() << "Message from" << message.sender() << ":" << message.content();
}

void MainWindow::onWizzReceived(const QString &fromUsername)
{
    // Show wizz notification with animation
    QMessageBox::information(this, "Wizz!", fromUsername + " vous a envoyé un Wizz!");
    
    // Perform shake animation
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
    // Create a sequential animation group with multiple shake movements
    if (m_wizzAnimation) {
        delete m_wizzAnimation;
    }

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);

    // Shake effect: move left, right, left, right, center
    for (int i = 0; i < 5; ++i) {
        QPropertyAnimation *anim = new QPropertyAnimation(this, "wizzOffset", this);
        anim->setDuration(50);

        if (i % 2 == 0) {
            anim->setEndValue(QPoint(-10, 0)); // Move left
        } else {
            anim->setEndValue(QPoint(10, 0));  // Move right
        }

        group->addAnimation(anim);
    }

    // Final animation to return to original position
    QPropertyAnimation *finalAnim = new QPropertyAnimation(this, "wizzOffset", this);
    finalAnim->setDuration(50);
    finalAnim->setEndValue(QPoint(0, 0));
    group->addAnimation(finalAnim);

    m_wizzAnimation = group;
    connect(m_wizzAnimation, &QAbstractAnimation::finished, m_wizzAnimation, &QObject::deleteLater);
    m_wizzAnimation->start();
}

