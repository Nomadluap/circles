#include "mainwindow.hpp"
#include "ui_mainwindow.h"

using namespace Circles;
using namespace Circles::GUI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto g = Graph::Graph::generateHexArray(33, 33);


    ui->packingview->setPacking(g, 535, 536, 0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
