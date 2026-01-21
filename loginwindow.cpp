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
    //passwordInput->setPlaceholderText("Mot de passe");

    loginButton = new QPushButton("Se connecter");
    auto layout = new QVBoxLayout;
    layout ->addWidget(usernameInput);
    //layout ->addWidget(passwordInput);
    layout ->addWidget(loginButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, [this](){
        emit loginRequested(usernameInput->text());
        //emit loginRequested(usernameInput->text(), passwordInput->test());
    });
}
