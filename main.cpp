#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "server.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    QStringList args;
    for (int i = 0; i < argc; ++i) args << argv[i];

    if (args.contains("--server")) {
        QCoreApplication app(argc, argv);
        Server server;
        if (!server.startServer(1234)) {
            return 1;
        }
        return app.exec();
    }

    QApplication app(argc, argv);
    // ... reste du code GUI ...

    QWidget window;
    window.setWindowTitle("Wizz Mania - Test");
    window.setMinimumSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *label = new QLabel("Bienvenue dans Wizz Mania !", &window);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *button = new QPushButton("Tester la connexion", &window);
    layout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked, [&]() {
        QMessageBox::information(&window, "Info", "L'environnement Qt est operationnel.");
    });

    window.show();

    return app.exec();
}
