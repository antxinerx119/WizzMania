#include "loginwindow.hpp"

#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget{parent}
    , m_busy(false)
{
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

    loginButton = new QPushButton("Se connecter");
    registerButton = new QPushButton("Inscription");

    auto *layout = new QVBoxLayout;
    layout->addWidget(serverHostInput);
    layout->addWidget(serverPortInput);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::submitLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegistration);
}

void LoginWindow::setServerEndpoint(const QString &host, quint16 port)
{
    serverHostInput->setText(host);
    serverPortInput->setText(QString::number(port));
}

void LoginWindow::setBusy(bool busy)
{
    m_busy = busy;

    serverHostInput->setEnabled(!busy);
    serverPortInput->setEnabled(!busy);
    usernameInput->setEnabled(!busy);
    passwordInput->setEnabled(!busy);
    loginButton->setEnabled(!busy);
    registerButton->setEnabled(!busy);
    loginButton->setText(busy ? "Connexion..." : "Se connecter");
}

void LoginWindow::submitLogin()
{
    if (m_busy) {
        return;
    }

    QString host;
    quint16 port = 0;
    if (!parseEndpoint(host, port)) {
        return;
    }

    emit loginRequested(usernameInput->text(), passwordInput->text(), host, port);
}

void LoginWindow::openRegistration()
{
    if (m_busy) {
        return;
    }

    QString host;
    quint16 port = 0;
    if (!parseEndpoint(host, port)) {
        return;
    }

    emit registerRequested(host, port);
}

bool LoginWindow::parseEndpoint(QString &host, quint16 &port) const
{
    host = serverHostInput->text().trimmed();
    const QString portText = serverPortInput->text().trimmed();

    if (host.isEmpty()) {
        QMessageBox::warning(const_cast<LoginWindow *>(this), "Connexion", "L'adresse du serveur est obligatoire.");
        return false;
    }

    bool ok = false;
    const uint parsedPort = portText.toUInt(&ok);
    if (!ok || parsedPort == 0 || parsedPort > 65535) {
        QMessageBox::warning(const_cast<LoginWindow *>(this), "Connexion", "Le port du serveur doit etre compris entre 1 et 65535.");
        return false;
    }

    port = static_cast<quint16>(parsedPort);
    return true;
}
