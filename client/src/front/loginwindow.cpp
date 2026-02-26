#include "loginwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget{parent}
{
    usernameInput = new QLineEdit;
    usernameInput->setPlaceholderText("Nom d'utilisateur");
<<<<<<< HEAD:client/src/front/loginwindow.cpp
    passwordInput = new QLineEdit;
    passwordInput->setPlaceholderText("Mot de passe");
    passwordInput->setEchoMode(QLineEdit::Password);
=======
    //à ajouter une fois que vous pouvez integrer le mot de passe
    //passwordInput = new QLineEdit;
    //passwordInput->setPlaceholderText("Mot de passe");
>>>>>>> origin/Front:loginwindow.cpp

    loginButton = new QPushButton("Se connecter");
    registerButton = new QPushButton("Inscription");
    auto layout = new QVBoxLayout;
<<<<<<< HEAD:client/src/front/loginwindow.cpp
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);
=======
    layout ->addWidget(usernameInput);
    //layout ->addWidget(passwordInput);
    layout ->addWidget(loginButton);
    layout ->addWidget(registerButton);
>>>>>>> origin/Front:loginwindow.cpp
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, [this](){
        emit loginRequested(usernameInput->text(), passwordInput->text());
    });

    connect(registerButton, &QPushButton::clicked, this, [this](){
        emit registerRequested();
    });
}
