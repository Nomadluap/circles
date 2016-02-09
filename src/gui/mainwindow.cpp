#include "mainwindow.hpp"
#include "ui_mainwindow.h"

using namespace Circles;
using namespace Circles::GUI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto singleFlower = std::make_shared<Graph::Graph>();
    singleFlower->addEdge(1, 2);
    singleFlower->addEdge(2, 3);
    singleFlower->addEdge(3, 4);
    singleFlower->addEdge(4, 5);
    singleFlower->addEdge(5, 6);
    singleFlower->addEdge(6, 1);
    for(int i = 1; i <= 6; ++i) singleFlower->addEdge(0, i);

    singleFlower->addEdge(1, 20);
    singleFlower->addEdge(2, 20);
    singleFlower->addEdge(2, 21);
    singleFlower->addEdge(3, 21);
    // singleFlower->addEdge(20, 21);


    ui->packingview->setPacking(singleFlower, 0, 1, 0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
