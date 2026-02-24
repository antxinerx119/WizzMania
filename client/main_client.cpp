#include "client/src/front/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("WizzMania");

    MainWindow window;
    window.setWindowTitle("WizzMania");
    window.show();

    return app.exec();
}
