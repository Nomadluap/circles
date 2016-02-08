#include "dualpackingview.hpp"
#include "ui_dualpackingview.h"
#include <QDebug>
#include <QPoint>
#include <cmath>
#include <QGLWidget>
#include <QGLFormat>

using namespace Circles;
using namespace Circles::GUI;

DualPackingView::DualPackingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualPackingView)
{
    ui->setupUi(this);
    ui->euclidview->scale(33.0, 33.0);
    ui->hyperview->scale(100.0, 100.0);

    ui->euclidview->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
    ui->hyperview->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));

    ui->euclidview->setRenderHint(QPainter::Antialiasing, true);
    ui->hyperview->setRenderHint(QPainter::Antialiasing, true);

    ui->euclidview->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->hyperview->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    ui->euclidview->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->hyperview->setDragMode(QGraphicsView::ScrollHandDrag);

    ui->euclidview->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->hyperview->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    ui->euclidview->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->hyperview->setDragMode(QGraphicsView::ScrollHandDrag);
}

DualPackingView::~DualPackingView()
{
    delete ui;
}

void DualPackingView::setPacking(std::shared_ptr<Packing::EuclidPacking> p)
{
    *(this->graph_) = p->graph();
    this->euclidPacking_ = p;
    this->hyperPacking_ = std::make_shared<Packing::HyperPacking>(this->graph_);

    // copy over the layout settings
    this->hyperPacking_->setCenterCircle(p->centerCircle());
    this->hyperPacking_->setFirstNeighbour(p->firstNeighbour());
    this->hyperPacking_->setFirstNeighbourAngle(p->firstNeighbourAngle());

    //repack teh new hyper packing.
    hyperRepackAndLayout();

    this->generateViews();

}

void DualPackingView::setPacking(std::shared_ptr<Packing::HyperPacking> p)
{
    *(this->graph_) = p->graph();
    this->hyperPacking_ = p;
    this->euclidPacking_ = std::make_shared<Packing::EuclidPacking>(this->graph_);

    // copy over the layout settings
    this->euclidPacking_->setCenterCircle(p->centerCircle());
    this->euclidPacking_->setFirstNeighbour(p->firstNeighbour());
    this->euclidPacking_->setFirstNeighbourAngle(p->firstNeighbourAngle());

    //repack and layout the new euclidean packing
    this->euclidRepackAndLayout();

    this->generateViews();
}

void DualPackingView::setPacking(std::shared_ptr<Graph::Graph> g, int centerCircle, int firstNeighbour, qreal angle)
{
    this->graph_ = g;
    this->hyperPacking_ = std::make_shared<Packing::HyperPacking>(this->graph_);
    this->euclidPacking_ = std::make_shared<Packing::EuclidPacking>(this->graph_);

    // establish view settings for both new packings
    this->euclidPacking_->setCenterCircle(centerCircle);
    this->euclidPacking_->setFirstNeighbour(firstNeighbour);
    this->euclidPacking_->setFirstNeighbourAngle(angle);

    this->hyperPacking_->setCenterCircle(centerCircle);
    this->hyperPacking_->setFirstNeighbour(firstNeighbour);
    this->hyperPacking_->setFirstNeighbourAngle(angle);

    // perform initial repacks and layouts.
    this->hyperRepackAndLayout();
    this->euclidRepackAndLayout();

    this->generateViews();
}

void DualPackingView::hyperRepackAndLayout()
{
    this->hyperPacking_->repack(this->epsilon_, this->hyperRadius_);
    this->hyperPacking_->layout();
}

void DualPackingView::euclidRepackAndLayout()
{
    this->euclidPacking_->repack(this->epsilon_, 1.0);
    this->euclidPacking_->layout();
}

void DualPackingView::setEpsilonMagnitude(qreal mag)
{
    this->epsilon_ = pow(10, mag);
}

void DualPackingView::setHyperRadius(qreal radius)
{
    this->hyperRadius_ = radius;
}

void DualPackingView::generateViews()
{
    qDebug() << "Generating Views...";
    this->hyperView_ = std::make_shared<View::PackingView>(this->hyperPacking_);
    this->euclidView_ = std::make_shared<View::PackingView>(this->euclidPacking_);

    connect(hyperView_.get(),
            View::PackingView::gotMouseEvent,
            this,
            DualPackingView::zoomHyperView);
    connect(euclidView_.get(),
            View::PackingView::gotMouseEvent,
            this,
            DualPackingView::zoomEuclideanView);

    qDebug() << "Displaying Views...";
    ui->euclidview->setScene(this->euclidView_.get());
    ui->hyperview->setScene(this->hyperView_.get());

    // ui->hyperview->fitInView(-1.5, -1.5, 3.0, 3.0, Qt::KeepAspectRatio);
}

void DualPackingView::zoomEuclideanView(int delta)
{
    this->euclidZoom_ = pow(ZOOM_PER_MOUSECOUNT, delta/120.0);
    ui->euclidview->scale(euclidZoom_, euclidZoom_);
}

void DualPackingView::zoomHyperView(int delta)
{
    this->hyperZoom_ = pow(ZOOM_PER_MOUSECOUNT, delta/120.0);
    ui->hyperview->scale(hyperZoom_, hyperZoom_);
}
