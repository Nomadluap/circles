// #include "ui/MainWindow.hpp"
#include <QApplication>
#include "gui/dualpackingview.hpp"
#include "graph/Graph.hpp"
#include <memory>

using namespace Circles;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
    GUI::DualPackingView w;

    auto singleFlower = std::make_shared<Graph::Graph>();
    singleFlower->addEdge(1, 2);
    singleFlower->addEdge(2, 3);
    singleFlower->addEdge(3, 4);
    singleFlower->addEdge(4, 5);
    singleFlower->addEdge(5, 6);
    singleFlower->addEdge(6, 1);
    for(int i = 1; i <= 6; ++i) singleFlower->addEdge(0, i);

    w.setPacking(singleFlower, 0, 1, 0.0);

    w.show();


    return a.exec();
    return 0;
}
