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

signals:
    void loginRequested(QString username, QString password);
    void registerRequested();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *registerButton;
};

#endif // LOGINWINDOW_H
