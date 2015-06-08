#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PFile.hpp"


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

    ui->view->setRenderHint(QPainter::Antialiasing, true);
    ui->view->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->view->setInteractive(false);
    ui->view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //create the view object
    p = new Packing(Packing::PackingType::EuclideanPacking);
    connect(ui->checkCenters, SIGNAL(toggled(bool)), this, SLOT(setDrawCenters(bool)));
    connect(ui->checkCircles, SIGNAL(toggled(bool)), this, SLOT(setDrawCircles(bool)));
    connect(ui->checkConnectors, SIGNAL(toggled(bool)), this, SLOT(setDrawLinks(bool)));
    connect(ui->checkIndicies, SIGNAL(toggled(bool)), this, SLOT(setDrawIndicies(bool)));
    connect(ui->spinZoom, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFile()));

    //set up default view
    p->addNode(new Node(123, QPointF(0, 0), 40.0));
    ui->view->setScene(this->p);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}

void MainWindow::setZoom(int zoom)
{
    qreal scale = qPow(1.50, qreal(zoom));
    QMatrix m;
    m.scale(scale, scale);
    ui->view->setMatrix(m);
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File", ".", "*.p");
    if(!filename.isNull()){
        qDebug() << filename;
        PFile f(filename);
        if(p != nullptr) delete p;
        p = f.generatePacking();
        ui->view->setScene(this->p);
    }
}

void MainWindow::setDrawCircles(bool b)
{
    this->p->setDrawCircles(b);
}

void MainWindow::setDrawCenters(bool b)
{
    this->p->setDrawCenters(b);
}

void MainWindow::setDrawIndicies(bool b)
{
    this->p->setDrawIndicies(b);
}

void MainWindow::setDrawLinks(bool b)
{
    this->p->setDrawLinks(b);
}
