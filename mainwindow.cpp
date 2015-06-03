#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "circle.h"

#include <QDebug>
#define PI 3.141592653589793238463

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->view->setRenderHint(QPainter::Antialiasing, true);
    ui->view->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->view->setInteractive(false);
    ui->view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->populateScene();
    ui->view->setScene(this->scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateScene(){
    scene = new QGraphicsScene();
//    Circle *c1 = new Circle(0, 0, 100);
//    c1->setCentermarkVisible(true);
//    c1->setColor(QColor(255, 0, 0));
//    scene->addItem(c1);
//    Circle *c2 = Circle::tangentTo(c1, 0.0, 90);
//    c2->setColor(QColor(0, 255, 0));
//    scene->addItem(c2);
//    QList<Circle*> circs;
//    circs.append(c2);
//    for(int i = 1; i < 5; i++){
//        Circle *c = Circle::tangentTo(c1, circs.at(i-1), 70*sqrt(i));
//        c->setColor(QColor(0, 0, 255));
//        circs.append(c);
//        scene->addItem(c);
//    }
    QList<Circle*> circs;
    //first circle
    Circle *c = new Circle(QPointF(0, 0), 40);
    c->setIndex(44444);
    c->setColor(QColor(255, 0, 0));
    circs.append(c);
    //20 circles wide
    for(int i = 1; i < 20; i++){
        c = Circle::tangentTo(circs.back(), 0.0, 40);
        circs.append(c);
    }
    //now start alternate rows.
    for(int row=1; row<20; row++){
        //do odd-numbered-row first
        //first element of col row only has one tangency
        c = Circle::tangentTo(circs.at(41*(row-1)), 2 * PI  / 3.0, 40);
        c->setColor(QColor(0, 255, 0));
        circs.append(c);
        for(int col = 1; col<21; col++){
            //tangent to previous circle and circle 41 before
            c = Circle::tangentTo(circs.back(), circs.at(41 * (row-1) + col-1), 40);
            circs.append(c);
        }
        //now even rows are tangent to (index-21, index-20)
        for(int col = 0; col < 20; col++){
            c = Circle::tangentTo(circs.at(41*(row-1)+col+20), circs.at(41*(row-1)+col+21), 40);
            circs.append(c);
        }
    }
    for(int i = 0; i < circs.length(); i++){
        circs.at(i)->setIndex(i);
        scene->addItem(circs.at(i));
    }

}
