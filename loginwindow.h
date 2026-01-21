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
    void loginRequested(QString username);
    //à ajouter une fois le mot de passe opérationnel
    //void loginRequested(QString username, QString motdepasse);

private:
    QLineEdit *usernameInput;
    //QLineEdit *passwordInput;
    QPushButton *loginButton;
};

#endif // LOGINWINDOW_H
