#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#pragma once

#include <QWidget>

class QPushButton;
class QLineEdit;

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

    void setServerEndpoint(const QString &host, quint16 port);
    void setBusy(bool busy);

signals:
    void loginRequested(QString username, QString password, QString host, quint16 port);
    void registerRequested(QString host, quint16 port);

private slots:
    void submitLogin();
    void openRegistration();

private:
    bool parseEndpoint(QString &host, quint16 &port) const;

    QLineEdit *serverHostInput;
    QLineEdit *serverPortInput;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *registerButton;
    bool m_busy;
};

#endif // LOGINWINDOW_H
