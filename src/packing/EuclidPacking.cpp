#include "EuclidPacking.hpp"
#include <QDebug>

#include "EuclidCircle.hpp"
using namespace Circles;
using namespace Circles::Packing;

const qreal PI = 3.141592653589793238462643383279502884;

Circles::Packing::EuclidPacking::EuclidPacking()
{
    this->_graph = std::make_shared<Graph::Graph>();
}

EuclidPacking::EuclidPacking(std::shared_ptr<Graph::Graph> g)
{
    this->_graph = g;
    this->spawnCircles();
}
EuclidPacking::EuclidPacking(const EuclidPacking& other)
{
    this->_graph = other._graph;
    this->_circles = other._circles;
}

EuclidPacking::EuclidPacking(Circles::Packing::EuclidPacking&& other)
{
    this->_graph = std::move(other._graph);
    this->_circles = std::move(other._circles);
}

EuclidPacking& EuclidPacking::operator=(const EuclidPacking& other)
{
    this->_graph = other._graph;
    this->_circles = other._circles;
    return *(this);
}

void EuclidPacking::layout(int centerCircle)
{
    QList<std::shared_ptr<Circle>> unplacedCircles = this->_circles.values(); //circles which have not yet been placed
        QList<std::shared_ptr<Circle>> placedCircles; //nodes which have been placed but do not have full flowers.
        QList<std::shared_ptr<Circle>> floweredCircles; //nodes for which their entire flower has been placed.
        //place the first circle
        bool foundCenterCircle = false;
        for(auto c: unplacedCircles){ //find the circle that is to be the center circle.
            if (c->index() == centerCircle){
                qDebug() << "Placing first circle #" << c->index() << " at (0, 0)";
                c->setCenter(QPointF(0, 0));
                placedCircles.append(c);
                unplacedCircles.removeAll(c);
                //place the second node to right of the first node.
                std::shared_ptr<Circle> d = neighbours(*c).first();

                //at least one of c and d need to have a full flower
                if(!(hasFullFlower(*c) || hasFullFlower(*d)) ){
                    int dindex = 1;
                    do{
                        d = neighbours(*c).at(dindex);
                        dindex++;
                    } while(!hasFullFlower(*d) && dindex < neighbours(*c).length());
                    //fail if we didn't find a proper d.
                    if(dindex >= neighbours(*c).length()){
                        qDebug() << "Neither C or D has full flower. Fail.";
                        return;
                    }
                }

                qreal h1 = c->radius();
                qreal h2 = d->radius();

                qreal s = h1+h2;
                d->setCenter(QPointF(s, 0));
                qDebug() << "Placing second circle #" << d->index() << " at (" <<
                            s << ", 0)";
                placedCircles.append(d);
                unplacedCircles.removeAll(d);
                foundCenterCircle = true;
                break;
            }
        }
        //fail if we don't find the specified center circle.
        if(!foundCenterCircle){
            qDebug() << "Could not find specified center circle. Fail";
            return;
        }
        //now continue until all circles have been placed...
        while(!unplacedCircles.empty()){
            bool nonFullOp = false;
            //qDebug() << "--NEXT NODE--";
            //find a placed node that does not have its full flower placed.
            //we require that the w node does in fact have a full flower.
            std::shared_ptr<Circle> w;
            int wIndex = 0;
            do{
                w = placedCircles.at(wIndex);
                wIndex++;
            } while(!hasFullFlower(*w) && wIndex < placedCircles.length());
            //if w does not have a full flower at this point, then we need to get creative
            if(!hasFullFlower(*w) && unplacedCircles.contains(neighbours(*w).first())){
                nonFullOp = true;
                //return;
            }
            //find a nbhr of w which has been placed.
            int nbhrIndex = 0;
            if(nonFullOp){
                nbhrIndex = neighbours(*w).length() - 1;
                while(unplacedCircles.contains(neighbours(*w).at(nbhrIndex))) nbhrIndex--;
            }
            else{
                //not an infinite loop since every placed node has at least one placed neibhour.
                while(unplacedCircles.contains(neighbours(*w).at(nbhrIndex))) nbhrIndex++;
            }
            //now continue going around the nodes until we find the first one that is unplaced
            //we also need to check if the full flower has been placed.
            bool fullFlower = false;
            int nbhrOrigin = nbhrIndex;
            while(!unplacedCircles.contains(neighbours(*w).at(nbhrIndex))){
                nbhrIndex = (nbhrIndex + 1) % neighbours(*w).length();
                //if we wrap completely around, then we know that w's full flower
                //has been placed.
                if(nbhrIndex == nbhrOrigin){
                    fullFlower = true;
                    break;
                }
            }
            //if the full flower is complete, then update the lists and try again
            //with a different node.
            if(fullFlower){
                placedCircles.removeAll(w);
                floweredCircles.append(w);
                continue;
            }
            std::shared_ptr<Circle> u;
            if(nonFullOp){
                if(nbhrIndex == neighbours(*w).length() - 1) u = neighbours(*w).first();
                else u = neighbours(*w).at(nbhrIndex+1);
            }
            else{
                if(nbhrIndex == 0) u = neighbours(*w).last();
                else u = neighbours(*w).at(nbhrIndex-1);
            }

            //now v becomes this "first unplaced node"
            std::shared_ptr<Circle> v = neighbours(*w).at(nbhrIndex);

            //find the angle <UWV=alpha
            qreal alpha = this->angle(w->center(), u->center(), v->center());
            //find the argument of u
            QPointF relU = u->center() - w->center();
            qreal beta = atan2(relU.y(), relU.x());

            //we need to determine if the nodes are currently being laid out in a
            //clockwise or anticlockwise manner. Thus we need to look at the two
            //previous unplaced nodes, and look at their relative angles.

            //this only works if w has two or more placed neibhours so far.
            int placedCount = 0;
            int isCCW = true;
            for(auto n: placedCircles + floweredCircles){
                if(neighbours(*w).contains(n)) placedCount++;
            }
            if(placedCount >= 2 && neighbours(*w).length() >= 3){
                //grab uprime
                std::shared_ptr<Circle> uprime;
                if(nonFullOp){
                    if(nbhrIndex == neighbours(*w).length() - 1){
                        uprime = neighbours(*w).at(1);
                    }
                    else if(nbhrIndex == neighbours(*w).length() - 2){
                        uprime = neighbours(*w).at(2);
                    }
                    else{
                        uprime = neighbours(*w).at(nbhrIndex + 2);
                    }
                }
                else{
                    if(nbhrIndex == 0){
                        uprime = neighbours(*w).at(neighbours(*w).length() - 2);
                    }
                    else if(nbhrIndex == 1){
                        uprime = neighbours(*w).last();
                    }
                    else{
                        uprime = neighbours(*w).at(nbhrIndex - 2);
                    }
                }

                //now look at angles of uprime and u
                QPointF relUPrime = uprime->center() - w->center();
                qreal betaprime = atan2(relUPrime.y(), relUPrime.x());
                //difference between angles should be less than PI radians
                qreal diff = fmod(betaprime - beta + 2*PI, 2*PI);
                if(diff < PI){
                    //betaprime is "ahead" of beta, so we should continue clockwise
                    isCCW = false;
                }
                else{
                    //betaprime is "behind" beta, so continue anticlockwise
                    isCCW = true;
                }
            }
            //then the actual argument of v is beta + alpha or beta - alpha
            qreal arg;
            if(isCCW) arg = fmod(beta+alpha+2*PI, 2*PI);
            else arg = fmod(beta-alpha+2*PI, 2 * PI);
            qreal r = w->radius() + v->radius();
            //now plot the point using sin and
            //remember that this point is an offset from w.
            QPointF pos(r*cos(arg), r*sin(arg));
            pos += w->center();
            //set the position of v
            v->setCenter(pos);
            //and update the lists
            unplacedCircles.removeAll(v);
            placedCircles.append(v);
            //and then we continue
        }

}

qreal EuclidPacking::angle(const QPointF& p, const QPointF& p1, const QPointF& p2) const
{
    qreal a = (p1 - p).manhattanLength();
    qreal b = (p2 - p).manhattanLength();
    qreal c = (p2 - p1).manhattanLength();

    qreal arg = (a*a + b*b - c*c)/(2.0 * a * b);
    qreal angle = acos(arg);
    return angle;

}



EuclidPacking& EuclidPacking::operator=(EuclidPacking&& other)
{
    this->_graph = std::move(other._graph);
    this->_circles = std::move(other._circles);
    return *(this);
}



void EuclidPacking::spawnCircles()
{
    for(int i: this->_graph->getNodes()){
        this->_circles.insert(i, std::make_shared<EuclidCircle>(i) );
    }
}


bool Circles::Packing::operator==(const EuclidPacking& lhs, const EuclidPacking& rhs)
{
    //Packings are equal if their graphs are equivalent and their circles have the same dimensions.
    bool equalGraphs = (lhs._graph == rhs._graph);
    auto ca = lhs.circles();
    auto cb = rhs.circles();
    bool equalCircles = (ca == cb);
    return equalGraphs && equalCircles;
}
