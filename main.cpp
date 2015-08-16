#include "mainwindow.h"
#include "ShapeSelector.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ShapeSelector s;
    s.show();

    return a.exec();
}
