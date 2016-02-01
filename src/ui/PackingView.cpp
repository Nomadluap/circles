#include "PackingView.hpp"

using namespace Circles;
using namespace Circles::Ui;

PackingView::PackingView()
{

}

void PackingView::setDrawCircles(bool state)
{
    this->_drawCircles = state;
}

void PackingView::setDrawCenters(bool state)
{
    this->_drawCenters = state;
}

void PackingView::setDrawIndices(bool state)
{
    this->_drawIndices = state;
}

void PackingView::setDrawConnectors(bool state)
{
    this->_drawConnectors = state;
}

void PackingView::setDrawColor(bool state)
{
    this->_drawColor = state;
}

void PackingView::setPacking(std::shared_ptr<Packing::Packing> p)
{

}
