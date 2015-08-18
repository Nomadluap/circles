#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PFile.hpp"
#include "SelectionPacking.hpp"
#include "Node.hpp"
#include "PackingView.hpp"


#include <QDebug>
#include <QtMath>
#include <QWidget>
#include <QFileDialog>
#define PI 3.141592653589793238463

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFile()));


    //nodes = Node::generateHexArray(QPointF(0, 0), 10, 10, 0.05);
    auto nodes = Node::generateHexArray(QRectF(-1.5, -1.5, 3.0, 3.0), 0.1);
    p = new Packing(nodes, PackingType::EuclideanPacking);
    ui->view->setPacking(p);



}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}


void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File", ".", "*.p");
    if(!filename.isNull()){ //if the file dialog isn't cancelled

        qDebug() << filename;
        PFile f(filename);
        if(p != nullptr) delete p;
        p = f.generatePacking();
        ui->view->setPacking(p);
    }
}

