// #include "ui/MainWindow.hpp"
#include <QApplication>
#include "gui/dualpackingview.hpp"
#include "gui/mainwindow.hpp"
#include "graph/Graph.hpp"
#include <memory>

using namespace Circles;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
    GUI::MainWindow w;


    w.show();


    return a.exec();
}
