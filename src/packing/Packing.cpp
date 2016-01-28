#include <algorithm>

#include "Packing.hpp"

using namespace Circles::Packing;




const Graph::Graph& Circles::Packing::Packing::graph() const
{
    return *(this->_graph);
}

QMap<int,std::shared_ptr<Circle> > Circles::Packing::Packing::circles() const
{
    QMap<int, std::shared_ptr<Circle> > c;
    for(int i: this->_circles.keys()){
        c.insert(i, this->_circles.value(i) );
    }
    return std::move(c);
}

const Circle& Circles::Packing::Packing::circle(int index) const
{
    return *(this->_circles.value(index));
}

qreal Circles::Packing::Packing::anglesum(const Circle &c) const
{
    //find the c node, get its neibhours, and add up the angles.
    QPointF o = c.center();
    o.isNull();
    int cc = c.index();
    QList<int> nbhrs(this->_graph->neighbours(cc));
    nbhrs.isEmpty();
    return 0.0; //TODO FINISH
}
