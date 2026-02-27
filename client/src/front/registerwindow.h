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
    //à ajouter une fois que les inscriptions pour etre faites
    //void RegisterRequested(QString username, QString motdepasse);
    void passRegister();

private:
    QLineEdit *usernameInput;
    //QLineEdit *passwordInput;
    QPushButton *registerButton;
};

#endif // REGISTERWINDOW_H
