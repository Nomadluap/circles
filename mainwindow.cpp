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



    QList<Node*> nodes;
    int s = 5;
    for(int i = 0; i < s; i++){
        nodes.append(new Node(i, QPointF(0, 0), 0.01));
    }
    for(int i = 1; i < s; i++){
        nodes.at(i)->addNeibhour(nodes.at(0));
         nodes.at(i)->addNeibhour(nodes.at((i+1)%s));
        nodes.at(i)->setPosition(QPointF(0, 0));
    }
    nodes.last()->addNeibhour(nodes.first());
    nodes.at(s-1)->addNeibhour(nodes.at(1));
    nodes.at(0)->setRadius(0.25);


    p = new Packing(nodes, PackingType::HyperbolicPacking);


    //p = new Packing(Node::generateHexArray(10, 0.05), PackingType::EuclideanPacking);
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

