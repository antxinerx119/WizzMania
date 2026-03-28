#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H
#pragma once

#include <QWidget>

class QPushButton;
class QLineEdit;

class RegisterWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterWindow(QWidget *parent = nullptr);

signals:
    void registerRequested(QString username, QString password);
    void backRequested();

private slots:
    void submitRegistration();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QPushButton *registerButton;
    QPushButton *backButton;
};

#endif // REGISTERWINDOW_H
