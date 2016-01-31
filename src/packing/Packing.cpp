#include <algorithm>
#include <cmath>
#include "Packing.hpp"

using namespace Circles::Packing;

const qreal PI = 3.141592653589793238462643383279502884;


void Circles::Packing::Packing::repack(qreal epsilon, qreal outerRadius)
{
    //first set all radii
    for(auto c: this->_circles.values()){
        c->setRadius(outerRadius);
    }
    //compute a list of all inner nodes

    QList<std::shared_ptr<Circle> > interior;
    for(auto c: this->_circles.values()){
        if(!this->_graph->isBoundary(c->index())) interior.append(c);
    }
    //Repacking loop
    bool done = false;
    while(!done){
        //recompute radii
        for(auto c: interior){
            qreal theta = this->anglesum(*c);

            //NEW CODE FROM PRACTICUM 3 PAGE 243
            int k = neighbours(*c).length();
            qreal r = c->radius();
            qreal beta = sin(theta / (2.0 * k));
            qreal delta = sin(PI / qreal(k));
            qreal rhat = (beta / (1.0 - beta)) * r;
            qreal p = ((1.0 - delta) / delta) * rhat;
            c->setRadius(p);
        }
        //check that all radii satisfy the epsilon-condition
        done = true;
        for(auto c: interior){
            if(fabs(this->anglesum(*c) - 2.0*PI) >= epsilon){
                done=false;
                break;
            }
        }
    }

}

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
    return c;
}

const Circle& Circles::Packing::Packing::circle(int index) const
{
    return *(this->_circles.value(index));
}

bool Circles::Packing::Packing::hasFullFlower(const Circle& c) const
{
    return this->_graph->hasFullFlower(c.index());
}

QList<std::shared_ptr<Circle> > Circles::Packing::Packing::neighbours(const Circle& c)
{
   //  this->circle(this->_graph->neighbours(c->index()))
    QList<std::shared_ptr<Circle> > circs;
    for(int i: this->_graph->neighbours(c.index())){
        circs.append(this->_circles.value(i));
    }
    return std::move(circs);
}

qreal Circles::Packing::Packing::anglesum(const Circle &c) const
{
    qreal partialSum = 0.0;
    //find the c node, get its neibhours, and add up the angles.
    QPointF o = c.center();
    int cc = c.index();
    QList<int> nbhrs((this->_graph->sortedNeighbours(cc)));
    if(!this->_graph->hasFullFlower(cc)) return 0.0; // if we don't have a full flower then we can't do anglesum.
    for(int i = 0; i <nbhrs.length(); ++i){
        QPointF p1 = this->_circles.value(nbhrs.at(i))->center();
        QPointF p2 = this->_circles.value(nbhrs.at( (i + 1) % nbhrs.length()) )->center();
        partialSum += this->angle(o, p1, p2);
    }
    qreal PI = atan(1)*4.0;
    return std::fmod(partialSum, 2.0*PI );
}
