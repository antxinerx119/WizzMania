#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H
#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;

class MessageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWindow(QWidget *parent = nullptr);
    void setUsername(const QString &name);

private slots:
    void sendMessage();

private:
    QLabel *welcomeLabel;
    QTextEdit *messageDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
};

#endif // MESSAGEWINDOW_H
