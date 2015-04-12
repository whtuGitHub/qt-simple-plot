#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Marco Scarpetta");
    a.setOrganizationDomain("marcoscarpetta.github.io");
    a.setApplicationName("sdvm");

    MainWindow w;
    w.show();

    return a.exec();
}
