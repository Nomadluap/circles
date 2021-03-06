#include <algorithm>
#include <cmath>
#include "Packing.hpp"
#include <QDebug>

#include "graph/Triangle.hpp"
using namespace Circles::Packing;

const qreal PI = 3.141592653589793238462643383279502884;


void Circles::Packing::Packing::repack(qreal epsilon, qreal outerRadius)
{
    qDebug();
    qDebug() << "Performing repack: epsilon=" << epsilon << " out.radius=" << outerRadius;
    //first set all radii
    for(auto c: this->circles_.values()){
        c->setRadius(outerRadius);
    }
    //compute a list of all inner nodes

    QList<std::shared_ptr<Circle> > interior;
    for(auto c: this->circles_.values()){
        if(!this->graph_->isBoundary(c->index())) interior.append(c);
    }
    //Repacking loop
    bool done = false;
    while(!done){
        //recompute radii
        for(auto c: interior){
            qreal theta = this->anglesum(*c);
//            qDebug() << "circle" << c->index() << "anglesum" << theta;
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
//    for(auto c: this->circles_.values()){
//        qDebug() << "\tCircle:" << c->index() << "radius:" << c->radius();
//    }

}

const Graph::Graph& Circles::Packing::Packing::graph() const
{
    return *(this->graph_);
}

QMap<int,std::shared_ptr<Circle> > Circles::Packing::Packing::circles() const
{
    QMap<int, std::shared_ptr<Circle> > c;
    for(int i: this->circles_.keys()){
        c.insert(i, this->circles_.value(i) );
    }
    return c;
}

const Circle& Circles::Packing::Packing::circle(int index) const
{
    return *(this->circles_.value(index));
}

bool Circles::Packing::Packing::hasFullFlower(const Circle& c) const
{
    return this->graph_->hasFullFlower(c.index());
}

QList<std::shared_ptr<Circle> > Circles::Packing::Packing::neighbours(const Circle& c)
{
   //  this->circle(this->_graph->neighbours(c->index()))
    QList<std::shared_ptr<Circle> > circs;
    for(int i: this->graph_->neighbours(c.index())){
        circs.append(this->circles_.value(i));
    }
    return std::move(circs);
}

void Circles::Packing::Packing::setCenterCircle(int index)
{
    this->centerCircle_ = index;
}

void Circles::Packing::Packing::setFirstNeighbour(int index)
{
    this->firstNeighbour_ = index;
}

void Circles::Packing::Packing::setFirstNeighbourAngle(qreal angle)
{
    this->firstNeighbourAngle_ = angle;
}

int Circles::Packing::Packing::centerCircle()
{
    return this->centerCircle_;
}

int Circles::Packing::Packing::firstNeighbour()
{
    return this->firstNeighbour_;
}

qreal Circles::Packing::Packing::firstNeighbourAngle()
{
    return this->firstNeighbourAngle_;
}

QPointF Circles::Packing::Packing::getXY(PackingCoordinate p)
{
    Graph::Triangle t = p.triangle();
    QPointF v1 = this->circle(t.p1()).projCenter();
    QPointF v2 = this->circle(t.p2()).projCenter();
    QPointF v3 = this->circle(t.p3()).projCenter();
    QPointF result;
    result = v1 * p.a() + v2 * p.b() + v3 * p.c();
    return result;
}

void Circles::Packing::Packing::removeCircle(int id)
{
    //first remove the circle itself
    this->circles_.remove(id);
    //then remove the node from the graph
    this->graph_->removeNode(id);
}

qreal Circles::Packing::Packing::anglesum(const Circle &c) const
{
    qreal partialSum = 0.0;
    //find the c node, get its neibhours, and add up the angles.
    qreal o = c.radius();
    int cc = c.index();
    QList<int> nbhrs((this->graph_->sortedNeighbours(cc)));
    if(!this->graph_->hasFullFlower(cc)) return 0.0; // if we don't have a full flower then we can't do anglesum.
    for(int i = 0; i <nbhrs.length(); ++i){
        qreal p1 = this->circles_.value(nbhrs.at(i))->radius();
        qreal p2 = this->circles_.value(nbhrs.at( (i + 1) % nbhrs.length()) )->radius();
        partialSum += this->angle(o, p1, p2);
    }
    return partialSum;
}
