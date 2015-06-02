#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "circle.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->populateScene();
    ui->view->setScene(this->scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateScene(){
    scene = new QGraphicsScene();
    QGraphicsItem *item = new Circle(0, 0, 40);
    item->setPos(0, 0);
    scene->addItem(item);

}
