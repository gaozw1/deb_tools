#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QStringLiteral("Deb Tools"));
    w.setWindowIcon(QIcon("://deb_tools.svg"));
    w.show();

    return a.exec();
}
