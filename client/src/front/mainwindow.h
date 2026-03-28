#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QStringList>
#include "../back/ClientNetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QStackedWidget;
class LoginWindow;
class RegisterWindow;
class MessageWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QPoint wizzOffset READ wizzOffset WRITE setWizzOffset)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPoint wizzOffset() const { return m_wizzOffset; }
    void setWizzOffset(const QPoint &offset);

private slots:
    void onLoginRequested(const QString &username, const QString &password, const QString &host, quint16 port);
    void onRegistrationPageRequested(const QString &host, quint16 port);
    void onRegisterRequested(const QString &username, const QString &password, const QString &host, quint16 port);
    void onRegisterBackRequested(const QString &host, quint16 port);
    void onConnected();
    void onDisconnected();
    void onLoginSuccess(const QString &username);
    void onLoginFailed(const QString &error);
    void onUserListReceived(const QStringList &users);
    void onUserJoined(const QString &username);
    void onUserLeft(const QString &username);
    void onMessageReceived(const Message &message);
    void onWizzReceived(const QString &fromUsername);
    void onNetworkError(const QString &errorMessage);
    void performWizzAnimation();

private:
    void startAuthentication(const QString &username, const QString &password, const QString &host, quint16 port, bool registrationFlow);
    void syncUserListDisplay();

    Ui::MainWindow *ui;
    ClientNetwork *m_network;
    QString m_currentUsername;
    QString m_currentPassword;
    QString m_serverHost;
    quint16 m_serverPort;
    QStringList m_onlineUsers;
    QPoint m_wizzOffset;
    QAbstractAnimation *m_wizzAnimation;
    QStackedWidget *m_stack;
    LoginWindow *m_loginWindow;
    RegisterWindow *m_registerWindow;
    MessageWindow *m_messagingWindow;
    bool m_hasAuthenticatedSession;
    bool m_registrationInProgress;
};
#endif // MAINWINDOW_H
