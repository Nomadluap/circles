#include "PackingView.hpp"

#include "packing/Packing.hpp"
#include <QList>

using namespace Circles;
using namespace Circles::View;

PackingView::PackingView():
    QGraphicsScene()
{

}

PackingView::PackingView(std::shared_ptr<Packing::Packing> p):
    QGraphicsScene()
{
    this->packing_ = p;
    this->rebuildGraphics();
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
    for(auto i: this->packing_->circles()){
        auto c = std::make_shared<View::GraphicCircle>(*i);
        this->graphicCircles_.append(c);
        this->addItem(c.get());
    }

}

void PackingView::rebuildCenters()
{

}

void PackingView::rebuildIndices()
{

}

void PackingView::rebuildConnectors()
{

}

void PackingView::rebuildColor()
{

}

void PackingView::wheelEvent(QGraphicsSceneWheelEvent* e)
{
    emit gotMouseEvent(e->delta());
    e->accept();
}
