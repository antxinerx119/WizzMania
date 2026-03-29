#include "mainwindow.hpp"
#include "loginwindow.hpp"
#include "messagewindow.hpp"
#include "registerwindow.hpp"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *stack = new QStackedWidget;

    auto *login = new LoginWindow;
    auto *registerwin = new RegisterWindow;
    auto *messaging = new MessageWindow;

    stack->addWidget(login);      // index 0
    stack->addWidget(registerwin); // index 1
    stack->addWidget(messaging);  // index 2

    setCentralWidget(stack);

    connect(login, &LoginWindow::loginRequested, this, [stack, messaging](QString username){ //ajouter QString password a cote de QString username
        messaging->setUsername(username);
        stack->setCurrentIndex(2); // changer de page
    });

    connect(login, &LoginWindow::registerRequested, this, [stack](){
        stack->setCurrentIndex(1);
    });

    connect(registerwin, &RegisterWindow::passRegister, this, [stack](){
        stack->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow()
{}

