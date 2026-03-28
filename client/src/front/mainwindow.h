#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
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
    void onLoginRequested(const QString &username, const QString &password);
    void onRegisterRequested(const QString &username, const QString &password);
    void onConnected();
    void onDisconnected();
    void onLoginSuccess(const QString &username);
    void onLoginFailed(const QString &error);
    void onMessageReceived(const Message &message);
    void onWizzReceived(const QString &fromUsername);
    void onNetworkError(const QString &errorMessage);
    void performWizzAnimation();

private:
    void startAuthentication(const QString &username, const QString &password, bool registrationFlow);

    Ui::MainWindow *ui;
    ClientNetwork *m_network;
    QString m_currentUsername;
    QString m_currentPassword;
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
