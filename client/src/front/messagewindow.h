#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H
#pragma once

#include <QWidget>
#include <QStringList>

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QListWidget;

class MessageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWindow(QWidget *parent = nullptr);
    void setUsername(const QString &name);
    void updateUserList(const QStringList &users);

    void displayMessage(const QString &sender, const QString &text);
    void displayNotification(const QString &text);

signals:
    void messageSent(const QString &text);

private slots:
    void sendMessage();

private:
    QLabel *welcomeLabel;
    QTextEdit *messageDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QListWidget *userListWidget;
};

#endif // MESSAGEWINDOW_H
