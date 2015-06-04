#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QtMath>
#include <QWidget>
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
    p = new EuclideanPacking();
    connect(ui->checkCenters, SIGNAL(toggled(bool)), this->p, SLOT(setDrawCenters(bool)));
    connect(ui->checkCircles, SIGNAL(toggled(bool)), this->p, SLOT(setDrawCircles(bool)));
    connect(ui->checkConnectors, SIGNAL(toggled(bool)), this->p, SLOT(setDrawLinks(bool)));
    connect(ui->checkIndicies, SIGNAL(toggled(bool)), this->p, SLOT(setDrawIndicies(bool)));
    connect(ui->spinZoom, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));

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
