#include <QApplication>
#include <QString>

#include "mainwindow.h"

using namespace medianFilter;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Median Filter");
    MainWindow mw(argc > 1 ? QString(argv[1]) : QString());
    mw.show();
    return app.exec();
}