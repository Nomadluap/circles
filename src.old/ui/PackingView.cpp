#include "PackingView.hpp"
#include "ui_PackingView.h"

#include <QDebug>
#include <QtMath>
#include <QWidget>
#include <QFileDialog>
#include <QGLWidget>

PackingView::PackingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PackingView)
{
    ui->setupUi(this);

    ui->view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
    ui->view->setRenderHint(QPainter::Antialiasing, true);
    ui->view->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->view->setInteractive(false);
    //ui->view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //create the view object
    connect(ui->checkCenters, SIGNAL(toggled(bool)), this, SLOT(setDrawCenters(bool)));
    connect(ui->checkCircles, SIGNAL(toggled(bool)), this, SLOT(setDrawCircles(bool)));
    connect(ui->checkConnectors, SIGNAL(toggled(bool)), this, SLOT(setDrawLinks(bool)));
    connect(ui->checkIndicies, SIGNAL(toggled(bool)), this, SLOT(setDrawIndicies(bool)));
    connect(ui->checkBoundary, SIGNAL(toggled(bool)), this, SLOT(setDrawBoundary(bool)));

    connect(ui->rdEuclidean, SIGNAL(toggled(bool)),
            this, SLOT(setPackingType(bool)));


    connect(ui->spinZoom, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
    connect(ui->rdSelectionMode, SIGNAL(clicked(bool)), this, SLOT(setSelectMode()));
    connect(ui->rdDragMode, SIGNAL(clicked(bool)), this, SLOT(setDragMode()));

    connect(ui->btnLayout, SIGNAL(clicked(bool)), this, SLOT(doLayout()));
    connect(ui->btnRepack, SIGNAL(clicked(bool)), this, SLOT(doRepack()));
    this->setZoom(ui->spinZoom->value());

    //set up prpoerty window
    propWindow = new PropertyWindow(this);
    propWindow->setModal(false);
    connect(ui->btnProperties, SIGNAL(clicked(bool)), propWindow, SLOT(show()));

}

PackingView::PackingView(Packing *p, QWidget *parent):
    PackingView(parent)
{
    this->setPacking(p);
}

PackingView::~PackingView()
{
    delete ui;
}

void PackingView::setPacking(Packing *p)
{
    PackingType type = p->getType();
    this->packing = p;
    this->ui->view->setScene(p);
    p->setDrawCenters(false);
    p->setDrawCircles(true);
    p->setDrawIndicies(false);
    p->setDrawLinks(false);
    p->setDrawBoundary(false);
    if(type == PackingType::EuclideanPacking) ui->rdEuclidean->setChecked(true);
    else ui->rdHyperbolic->setChecked(true);
    connect(packing, SIGNAL(newNodeSelected(Node*)),
            propWindow, SLOT(setNode(Node*)));
    //reset all checkboxes
    ui->checkBoundary->setChecked(false);
    ui->checkCenters->setChecked(false);
    ui->checkCircles->setChecked(true);
    ui->checkConnectors->setChecked(false);
    ui->checkIndicies->setChecked(false);
    if(p->centerCircleID != -1) ui->spinCenterCircleID->setValue(p->centerCircleID);

}

void PackingView::setZoom(int zoom)
{
    qreal scale = qPow(1.50, qreal(zoom));
    QMatrix m;
    m.scale(scale, scale);
    ui->view->setMatrix(m);
}

void PackingView::setDrawCircles(bool b)
{
    this->packing->setDrawCircles(b);
}

void PackingView::setDrawCenters(bool b)
{
    this->packing->setDrawCenters(b);
}

void PackingView::setDrawIndicies(bool b)
{
    this->packing->setDrawIndicies(b);
}

void PackingView::setDrawLinks(bool b)
{
    this->packing->setDrawLinks(b);
}

void PackingView::setDrawBoundary(bool b)
{
    this->packing->setDrawBoundary(b);
}

void PackingView::test()
{

}

void PackingView::doLayout()
{
    int index = ui->spinCenterCircleID->value();
    packing->layout(index);
}

void PackingView::doRepack()
{
    qreal outerRadius = ui->spinOuterRadius->value();
    qreal epsilon = ui->spinEpsilon->value();
    packing->repack(epsilon, outerRadius);
}

void PackingView::setDragMode()
{
    ui->view->setInteractive(false);
    ui->view->setDragMode(QGraphicsView::ScrollHandDrag);
}

void PackingView::setSelectMode()
{
//    ui->view->setDragMode(QGraphicsView::RubberBandDrag);
    ui->view->setDragMode(QGraphicsView::NoDrag);
    ui->view->setInteractive(true);
}

void PackingView::setPackingType(bool b)
{
    qDebug() << "Setting Packing type";
    if(b) this->packing->setPackingType(PackingType::EuclideanPacking);
    else this->packing->setPackingType(PackingType::HyperbolicPacking);
}


