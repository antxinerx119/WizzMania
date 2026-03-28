#include "registerwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget{parent}
{
    auto *titleLabel = new QLabel("Creer un compte");

    usernameInput = new QLineEdit;
    usernameInput->setPlaceholderText("Nom d'utilisateur");

    passwordInput = new QLineEdit;
    passwordInput->setPlaceholderText("Mot de passe");
    passwordInput->setEchoMode(QLineEdit::Password);

    confirmPasswordInput = new QLineEdit;
    confirmPasswordInput->setPlaceholderText("Confirmer le mot de passe");
    confirmPasswordInput->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("Creer le compte");
    backButton = new QPushButton("Retour");

    auto *layout = new QVBoxLayout;
    layout->addWidget(titleLabel);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(confirmPasswordInput);
    layout->addWidget(registerButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::submitRegistration);
    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::backRequested);
}

void RegisterWindow::submitRegistration()
{
    const QString username = usernameInput->text().trimmed();
    const QString password = passwordInput->text();
    const QString confirmPassword = confirmPasswordInput->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Inscription", "Le nom d'utilisateur est obligatoire.");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Inscription", "Le mot de passe est obligatoire.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Inscription", "Les mots de passe ne correspondent pas.");
        return;
    }

    emit registerRequested(username, password);
}
