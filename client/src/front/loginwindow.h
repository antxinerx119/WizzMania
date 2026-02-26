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
<<<<<<< HEAD:client/src/front/loginwindow.h
    void loginRequested(QString username, QString password);
=======
    void loginRequested(QString username);
    void registerRequested();
    //à ajouter une fois le mot de passe opérationnel
    //void loginRequested(QString username, QString motdepasse);
>>>>>>> origin/Front:loginwindow.h

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *registerButton;
};

#endif // LOGINWINDOW_H
