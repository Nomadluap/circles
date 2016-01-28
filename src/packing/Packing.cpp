#include <algorithm>
#include <cmath>
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
    qreal partialSum = 0.0;
    //find the c node, get its neibhours, and add up the angles.
    QPointF o = c.center();
    int cc = c.index();
    QList<int> nbhrs(this->_graph->sortedNeighbours(cc));
    if(!this->_graph->hasFullFlower(cc)) return 0.0; // if we don't have a full flower then we can't do anglesum.
    for(int i = 0; i <nbhrs.length(); ++i){
        QPointF p1 = this->_circles.value(nbhrs.at(i))->center();
        QPointF p2 = this->_circles.value(nbhrs.at( (i + 1) % nbhrs.length()) )->center();
        partialSum += this->angle(o, p1, p2);
    }
    qreal PI = atan(1)*4.0;
    return std::fmod(partialSum, 2.0*PI );
}
