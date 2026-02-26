#include "loginwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget{parent}
{
    usernameInput = new QLineEdit;
    usernameInput->setPlaceholderText("Nom d'utilisateur");
    //à ajouter une fois que vous pouvez integrer le mot de passe
    //passwordInput = new QLineEdit;
    //passwordInput->setPlaceholderText("Mot de passe");

    loginButton = new QPushButton("Se connecter");
    registerButton = new QPushButton("Inscription");
    auto layout = new QVBoxLayout;
    layout ->addWidget(usernameInput);
    //layout ->addWidget(passwordInput);
    layout ->addWidget(loginButton);
    layout ->addWidget(registerButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, [this](){
        emit loginRequested(usernameInput->text());
        //emit loginRequested(usernameInput->text(), passwordInput->test());
    });

    connect(registerButton, &QPushButton::clicked, this, [this](){
        emit registerRequested();
    });
}
