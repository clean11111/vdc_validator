#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int x = w.width() * 2;
    int y = w.minimumHeight();
    w.resize(x,y);
    w.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    w.show();

    return a.exec();
}
