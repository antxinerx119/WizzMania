#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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
