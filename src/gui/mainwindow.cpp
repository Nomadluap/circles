#include "mainwindow.hpp"
#include "ui_mainwindow.h"

using namespace Circles;
using namespace Circles::GUI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto g = Graph::Graph::generateHexArray(10, 10);


    ui->packingview->setPacking(g, 53, 54, 0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
