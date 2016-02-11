// #include "ui/MainWindow.hpp"
#include <QApplication>
#include "gui/dualpackingview.hpp"
#include "gui/mainwindow.hpp"
#include "graph/Graph.hpp"
#include <memory>
#include "QPixmapCache"

using namespace Circles;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QPixmapCache::setCacheLimit(102400);
    GUI::MainWindow w;


    w.show();


    return a.exec();
}
