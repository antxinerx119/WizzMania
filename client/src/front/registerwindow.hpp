#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H
#pragma once

#include <QWidget>

class QPushButton;
class QLineEdit;

class RegisterWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterWindow(QWidget *parent = nullptr);

    void setServerEndpoint(const QString &host, quint16 port);
    void setBusy(bool busy);

signals:
    void registerRequested(QString username, QString password, QString host, quint16 port);
    void backRequested(QString host, quint16 port);

private slots:
    void submitRegistration();
    void goBack();

private:
    bool parseEndpoint(QString &host, quint16 &port) const;

    QLineEdit *serverHostInput;
    QLineEdit *serverPortInput;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QPushButton *registerButton;
    QPushButton *backButton;
    bool m_busy;
};

#endif // REGISTERWINDOW_H
