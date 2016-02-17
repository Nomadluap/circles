#include "PackingView.hpp"

#include "packing/Packing.hpp"
#include "packing/EuclidPacking.hpp"
#include <QList>

#include "graph/Triangle.hpp"
#include "graph/Graph.hpp"
#include "packing/PackingCoordinate.hpp"
#include <QDebug>
#include <QGraphicsSimpleTextItem>

using namespace Circles;
using namespace Circles::View;
using Circles::Packing::PackingCoordinate;

PackingView::PackingView():
    QGraphicsScene()
{
    this->setItemIndexMethod(QGraphicsScene::NoIndex);
}

PackingView::PackingView(std::shared_ptr<Packing::Packing> p):
    QGraphicsScene()
{
    this->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->packing_ = p;
    this->rebuildGraphics();
}



void PackingView::setTriangleColor(Graph::Triangle t, QColor c)
{
    this->triangleColors_.insert(t, c);
}

void PackingView::clearTriangleColor()
{
    this->triangleColors_.clear();
}

Packing::Packing& PackingView::packing()
{
    return *(this->packing_);
}

void PackingView::setDrawCircles(bool state)
{
    this->drawCircles_ = state;
}

void PackingView::setDrawCenters(bool state)
{
    this->drawCenters_ = state;
}

void PackingView::setDrawIndices(bool state)
{
    this->drawIndices_ = state;
}

void PackingView::setDrawConnectors(bool state)
{
    this->drawConnectors_ = state;
}

void PackingView::setDrawColor(bool state)
{
    this->drawColor_ = state;
}

void PackingView::setPacking(std::shared_ptr<Packing::Packing> p)
{
    this->packing_ = p;
    this->rebuildGraphics();
    emit(packingChanged(p));
}

void PackingView::rebuildGraphics()
{
    // Rebuild each graphics item class in turn
    // the draw order has to be correct -- this is where it's defined.
    this->rebuildColor();
    this->rebuildCircles();
    this->rebuildConnectors();
    this->rebuildCenters();
    this->rebuildIndices();
}

void PackingView::rebuildCircles()
{
    // remove the old
    for(auto i: this->graphicCircles_) this->removeItem(i.get());
    this->graphicCircles_.clear();

    // and rebuild.
    if(this->drawCircles_){
        for(auto i: this->packing_->circles()){
            auto c = std::make_shared<View::GraphicCircle>(*i);
            this->graphicCircles_.append(c);
            this->addItem(c.get());
        }
    }

}

void PackingView::rebuildCenters()
{

}

void PackingView::rebuildIndices()
{
    for(auto i: this->indices_) this->removeItem(i.get());
    this->indices_.clear();

    if(this->drawIndices_){
        for(auto c: this->graphicCircles_){
            auto t = std::make_shared<Text>(QString::number(c->graphIndex()));
            t->setFont(QFont("Times", 10));
            t->setPos(this->packing_->circle(c->graphIndex()).projCenter());
         //   t->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
            this->indices_.append(t);
            this->addItem(t.get());
        }
    }
}

void PackingView::rebuildConnectors()
{

}

void PackingView::rebuildColor()
{
    // remove the old
    for(auto i: this->colorPolygons_) this->removeItem(i.get());
    this->colorPolygons_.clear();
    // and rebuild
    if(this->drawColor_){
        // for some reason we can't use key-based retrevial, as it returns some invalid colors.
        // May have something to do with the ordering of the triangles.
        //for(Graph::Triangle t: this->triangleColors_.keys()){
        for(int i = 0; i < this->triangleColors_.keys().length(); ++i){
            Graph::Triangle t = this->triangleColors_.keys().at(i);
            QColor color = this->triangleColors_.values().at(i);
            QPointF v1 = this->packing_->circle(t.p1()).center();
            QPointF v2 = this->packing_->circle(t.p2()).center();
            QPointF v3 = this->packing_->circle(t.p3()).center();
            QPolygonF poly;
            poly << v1 << v2 << v3 << v1;

            auto pp = std::make_shared<QGraphicsPolygonItem>(poly);
            pp->setBrush(QBrush(color));
            pp->setPen(QPen(QColor(0, 0, 0, 0)));
            colorPolygons_.append(pp);
            this->addItem(pp.get());
        }
    }
}

void PackingView::wheelEvent(QGraphicsSceneWheelEvent* e)
{
    emit gotMouseWheelEvent(e->delta());
    e->accept();
}

void PackingView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit gotMousePressEvent(event);
}
