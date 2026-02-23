#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QStringList>
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
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
        return app.exec();
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
