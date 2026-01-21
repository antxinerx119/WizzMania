#include "messagewindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

MessageWindow::MessageWindow(QWidget *parent)
    : QWidget{parent}
{
    welcomeLabel = new QLabel("Bienvenue !");

    messageDisplay = new QTextEdit;
    messageDisplay->setReadOnly(true);

    messageInput = new QLineEdit;
    messageInput->setPlaceholderText("Écris ton message ici...");

    sendButton = new QPushButton("Envoyer");

    auto inputLayout = new QHBoxLayout;
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    auto layout = new QVBoxLayout;
    layout->addWidget(welcomeLabel);
    layout->addWidget(messageDisplay);
    layout->addLayout(inputLayout);
    setLayout(layout);

    connect(sendButton, &QPushButton::clicked, this, &MessageWindow::sendMessage);
}

void MessageWindow::setUsername(const QString &name) {
    welcomeLabel->setText("Bienvenue " + name);
}

void MessageWindow::sendMessage() {
    QString text = messageInput->text().trimmed();
    if (!text.isEmpty()) {
        messageDisplay->append("<b>Moi :</b> " + text);
        messageInput->clear();
    }
}
