#include "calculator.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "Starting application...";

    Calculator calc;
    calc.show();

    qDebug() << "Application started";

    return app.exec();
}
