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

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
};

#endif // LOGINWINDOW_H
