#include "messagewindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSplitter>

MessageWindow::MessageWindow(QWidget *parent)
    : QWidget{parent}
{
    welcomeLabel = new QLabel("Bienvenue !");

    messageDisplay = new QTextEdit;
    messageDisplay->setReadOnly(true);

    messageInput = new QLineEdit;
    messageInput->setPlaceholderText("Écris ton message ici...");

    sendButton = new QPushButton("Envoyer");

    // User list widget
    userListWidget = new QListWidget;
    userListWidget->setMaximumWidth(200);
    userListWidget->setAlternatingRowColors(true);

    auto inputLayout = new QHBoxLayout;
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    auto chatLayout = new QVBoxLayout;
    chatLayout->addWidget(welcomeLabel);
    chatLayout->addWidget(messageDisplay);
    chatLayout->addLayout(inputLayout);

    // Splitter for chat and user list
    auto splitter = new QSplitter(Qt::Horizontal);
    auto chatWidget = new QWidget;
    chatWidget->setLayout(chatLayout);
    splitter->addWidget(chatWidget);
    splitter->addWidget(userListWidget);
    splitter->setSizes({600, 200});

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &MessageWindow::sendMessage);
}

void MessageWindow::setUsername(const QString &name) {
    welcomeLabel->setText("Bienvenue " + name);
}

void MessageWindow::updateUserList(const QStringList &users) {
    userListWidget->clear();
    userListWidget->addItems(users);
}

void MessageWindow::sendMessage() {
    QString text = messageInput->text().trimmed();
    if (!text.isEmpty()) {
        emit messageSent(text);
        messageInput->clear();
    }
}

void MessageWindow::displayMessage(const QString &sender, const QString &text) {
    messageDisplay->append("<b>" + sender + " :</b> " + text);
}

void MessageWindow::displayNotification(const QString &text) {
    messageDisplay->append("<i><font color='gray'>" + text + "</font></i>");
}
