#include <algorithm>

#include "Packing.hpp"

using namespace Circles::Packing;




const Graph::Graph& Circles::Packing::Packing::graph() const
{
    return *(this->_graph);
}

QMap<int, Circle *> Circles::Packing::Packing::circles() const
{
    QMap<int, Circle* > c;
    for(int i: this->_circles.keys()){
        c.insert(i, this->_circles.value(i).get());
    }
    return std::move(c);
}

const Circle * Circles::Packing::Packing::circle(int index) const
{
    Circle* c = this->_circles.value(index).get();
    return c;
}

qreal Circles::Packing::Packing::anglesum(const Circle &c) const
{
    //find the c node, get its neibhours, and add up the angles.
    QPointF o = c.center();
    int cc = c.index();
    QList<int> neighbours(this->_graph->neighbours(cc));
}
