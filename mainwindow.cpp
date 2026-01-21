#include "mainwindow.h"
#include "loginwindow.h"
#include "messagewindow.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *stack = new QStackedWidget;

    auto *login = new LoginWindow;
    auto *messaging = new MessageWindow;

    stack->addWidget(login);      // index 0
    stack->addWidget(messaging);  // index 1

    setCentralWidget(stack);

    connect(login, &LoginWindow::loginRequested, this, [stack, messaging](QString username){ //ajouter QString password a cote de QString username
        messaging->setUsername(username);
        stack->setCurrentIndex(1); // changer de page
    });
}

MainWindow::~MainWindow()
{}

