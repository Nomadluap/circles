#include "mainwindow.hpp"
#include "ui_mainwindow.h"

using namespace Circles;
using namespace Circles::GUI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto g = Graph::Graph::generateHexArray(5, 5);


    ui->packingview->setPacking(g, 12, 13, 0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
