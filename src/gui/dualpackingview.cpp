#include "dualpackingview.hpp"
#include "ui_dualpackingview.h"
#include <QDebug>
#include <QPoint>
#include <cmath>
#include <QGLWidget>
#include <QGLFormat>
#include <memory>

#include "view/HyperColor.hpp"

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

    ui->euclidview->setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    ui->hyperview->setOptimizationFlag(QGraphicsView::DontSavePainterState, true);

    ui->euclidview->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->hyperview->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    ui->euclidview->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->hyperview->setDragMode(QGraphicsView::ScrollHandDrag);

    ui->euclidview->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    ui->hyperview->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

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

void DualPackingView::repackAndLayout()
{
    this->hyperRepackAndLayout();
    this->euclidRepackAndLayout();
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
            View::PackingView::gotMouseWheelEvent,
            this,
            DualPackingView::zoomHyperView);
    connect(euclidView_.get(),
            View::PackingView::gotMouseWheelEvent,
            this,
            DualPackingView::zoomEuclideanView);

    qDebug() << "Displaying Views...";
    ui->euclidview->setScene(this->euclidView_.get());
    ui->hyperview->setScene(this->hyperView_.get());

    this->generateHyperColors(*(this->hyperView_));
    this->generateHyperColors(*(this->euclidView_));

    this->euclidView_->rebuildGraphics();
    this->hyperView_->rebuildGraphics();

    // ui->hyperview->fitInView(-1.5, -1.5, 3.0, 3.0, Qt::KeepAspectRatio);
}

void DualPackingView::generateHyperColors(View::PackingView& pv)
{
    // iterate over the triangles in p's graph
    for(auto t: pv.packing().graph().triangles()){
        //generate a coordinate for the center of the triangle
        Packing::PackingCoordinate coord(t, 1.0/3.0, 1.0/3.0, 1.0/3.0);
        //convert that coord to an xy in the hyperbolic packing
        QPointF xy = this->hyperPacking_->getXY(coord);
        //now pull a color from that xy
        QColor c = Circles::View::getHyperColor(xy);
        pv.setTriangleColor(t, c);

    }
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

void DualPackingView::setEpsilon(qreal magnitude)
{
    this->epsilon_ = pow(10.0, magnitude);
}

void DualPackingView::setFirstCircle(int id)
{
    this->euclidPacking_->setCenterCircle(id);
    this->hyperPacking_->setCenterCircle(id);
}

void DualPackingView::setFirstNeighbour(int id)
{
    this->euclidPacking_->setFirstNeighbour(id);
    this->hyperPacking_->setFirstNeighbour(id);
}

void DualPackingView::setFirstNeighbourAngle(int slidervalue)
{
    qreal theta = double(270 - slidervalue) * 3.141592654 / 180.0;
    this->euclidPacking_->setFirstNeighbourAngle(theta);
    this->hyperPacking_->setFirstNeighbourAngle(theta);
}

void DualPackingView::setDrawColors(bool state)
{
    this->euclidView_->setDrawColor(state);
    this->hyperView_->setDrawColor(state);
}

void DualPackingView::setDrawCircles(bool state)
{
    this->euclidView_->setDrawCircles(state);
    this->hyperView_->setDrawCircles(state);
}

void DualPackingView::setDrawIndices(bool state)
{
    this->euclidView_->setDrawIndices(state);
    this->hyperView_->setDrawIndices(state);
}
