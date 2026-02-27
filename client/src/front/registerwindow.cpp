#include "registerwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget{parent}
{
    usernameInput = new QLineEdit;
    usernameInput->setPlaceholderText("Nom d'utilisateur");
    //à ajouter une fois que vous pouvez integrer le mot de passe
    //passwordInput = new QLineEdit;
    //passwordInput->setPlaceholderText("Mot de passe");

    registerButton = new QPushButton("Inscription");
    auto layout = new QVBoxLayout;
    layout ->addWidget(usernameInput);
    //layout ->addWidget(passwordInput);
    layout ->addWidget(registerButton);
    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, [this](){
        emit passRegister();
    });
}
