#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "circle.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->view->setRenderHint(QPainter::Antialiasing, true);
    this->populateScene();
    ui->view->setScene(this->scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateScene(){
    qDebug() << "starting layout \n";
    scene = new QGraphicsScene();
    Circle *c1 = new Circle(0, 0, 40);
    c1->setCentermarkVisible(true);
    c1->setColor(QColor(255, 0, 0));
    scene->addItem(c1);
    Circle *c2 = Circle::tangentTo(c1, 0.0, 40);
    c2->setColor(QColor(0, 255, 0));
    scene->addItem(c2);
    QList<Circle*> circs;
    circs.append(c2);
    for(int i = 1; i < 5; i++){
        Circle *c = Circle::tangentTo(c1, circs.at(i-1), 40*sqrt(i));
        c->setColor(QColor(0, 0, 255));
        circs.append(c);
        scene->addItem(c);
    }

    qDebug() << "finished laying out \n";
}
