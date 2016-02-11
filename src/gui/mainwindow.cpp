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

    connect(ui->actionOpen_Selector,
            SIGNAL(triggered(bool)),
            this,
            SLOT(openSelector()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSelector()
{
    ss = std::make_shared<Circles::GUI::ShapeSelector>();
    connect(ss.get(),
            SIGNAL(packingAccepted(std::shared_ptr<Packing::EuclidPacking>)),
            ui->packingview,
            SLOT(setPacking(std::shared_ptr<Packing::EuclidPacking>)));

    ss->show();
}
