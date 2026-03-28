#include "registerwindow.h"

#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget{parent}
{
    auto *titleLabel = new QLabel("Creer un compte");

    serverHostInput = new QLineEdit;
    serverHostInput->setPlaceholderText("Adresse du serveur");
    serverHostInput->setText("127.0.0.1");

    serverPortInput = new QLineEdit;
    serverPortInput->setPlaceholderText("Port du serveur");
    serverPortInput->setValidator(new QIntValidator(1, 65535, this));
    serverPortInput->setText("12345");

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
    layout->addWidget(serverHostInput);
    layout->addWidget(serverPortInput);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(confirmPasswordInput);
    layout->addWidget(registerButton);
    layout->addWidget(backButton);
    setLayout(layout);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::submitRegistration);
    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::goBack);
}

void RegisterWindow::setServerEndpoint(const QString &host, quint16 port)
{
    serverHostInput->setText(host);
    serverPortInput->setText(QString::number(port));
}

void RegisterWindow::submitRegistration()
{
    QString host;
    quint16 port = 0;
    if (!parseEndpoint(host, port)) {
        return;
    }

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

    emit registerRequested(username, password, host, port);
}

void RegisterWindow::goBack()
{
    QString host;
    quint16 port = 0;
    if (!parseEndpoint(host, port)) {
        return;
    }

    emit backRequested(host, port);
}

bool RegisterWindow::parseEndpoint(QString &host, quint16 &port) const
{
    host = serverHostInput->text().trimmed();
    const QString portText = serverPortInput->text().trimmed();

    if (host.isEmpty()) {
        QMessageBox::warning(const_cast<RegisterWindow *>(this), "Inscription", "L'adresse du serveur est obligatoire.");
        return false;
    }

    bool ok = false;
    const uint parsedPort = portText.toUInt(&ok);
    if (!ok || parsedPort == 0 || parsedPort > 65535) {
        QMessageBox::warning(const_cast<RegisterWindow *>(this), "Inscription", "Le port du serveur doit etre compris entre 1 et 65535.");
        return false;
    }

    port = static_cast<quint16>(parsedPort);
    return true;
}
