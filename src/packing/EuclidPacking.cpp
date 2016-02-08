#include "EuclidPacking.hpp"
#include <QDebug>

#include "EuclidCircle.hpp"
using namespace Circles;
using namespace Circles::Packing;

const qreal PI = 3.141592653589793238462643383279502884;

Circles::Packing::EuclidPacking::EuclidPacking()
{
    this->graph_ = std::make_shared<Graph::Graph>();
}

EuclidPacking::EuclidPacking(std::shared_ptr<Graph::Graph> g)
{
    this->graph_ = g;
    this->spawnCircles();
}
EuclidPacking::EuclidPacking(const EuclidPacking& other)
{
    this->graph_ = other.graph_;
    this->circles_ = other.circles_;
}

EuclidPacking::EuclidPacking(Circles::Packing::EuclidPacking&& other)
{
    this->graph_ = std::move(other.graph_);
    this->circles_ = std::move(other.circles_);
}

EuclidPacking& EuclidPacking::operator=(const EuclidPacking& other)
{
    this->graph_ = other.graph_;
    this->circles_ = other.circles_;
    return *(this);
}

void EuclidPacking::layout()
{
    QList<std::shared_ptr<Circle>> unplacedCircles = this->circles_.values(); //circles which have not yet been placed
    QList<std::shared_ptr<Circle>> placedCircles; //nodes which have been placed but do not have full flowers.
    QList<std::shared_ptr<Circle>> floweredCircles; //nodes for which their entire flower has been placed.
    //place the first circle
    bool foundCenterCircle = false;
    for(auto c: unplacedCircles){ //find the circle that is to be the center circle.
        if (c->index() == this->centerCircle_){
            qDebug() << "Placing first circle #" << c->index() << " at (0, 0)";
            c->setCenter(QPointF(0, 0));
            placedCircles.append(c);
            unplacedCircles.removeAll(c);
            //place the second node to right of the first node.
            std::shared_ptr<Circle> d = this->circles_.value(this->firstNeighbour_);

            if(!neighbours(*c).contains(d)){
                qDebug() << "centerCircle and FirstNeighbour are not neighbouring circles. Fail.";
                return;
            }
            //at least one of c and d need to have a full flower
            if(!(hasFullFlower(*c) || hasFullFlower(*d)) ){
                qDebug() << "Neither C or D has full flower. Fail.";
                return;
            }

            qreal h1 = c->radius();
            qreal h2 = d->radius();

            qreal s = h1+h2;
            //rotate correctly.
            QPointF dcenter(s * cos(-this->firstNeighbourAngle_), s * sin(-this->firstNeighbourAngle_) );
            d->setCenter(dcenter);
            qDebug() << "Placing second circle #" << d->index() << " at (" <<
                        dcenter.x() << ", " << dcenter.y() << ")";
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

        //now v becomes this "first unplaced node"
        std::shared_ptr<Circle> v = neighbours(*w).at(nbhrIndex);

        std::shared_ptr<Circle> u; //already placed node which is a neighbour to both w and v.
        if(nonFullOp){
            if(nbhrIndex == neighbours(*w).length() - 1) u = neighbours(*w).first();
            else u = neighbours(*w).at(nbhrIndex+1);
            // if for some reason u and v aren't neighbours, then we have to find a u that is.
            if(!neighbours(*v).contains(u)) u = neighbours(*w).at(nbhrIndex-1);

        }
        else{
            if(nbhrIndex == 0) u = neighbours(*w).last();
            else u = neighbours(*w).at(nbhrIndex-1);
        }



        //find the angle <UWV=alpha
        qreal alpha = this->angle(w->radius(), u->radius(), v->radius());
        qDebug() << "circle" << v->index() << "angle=" << alpha;
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
        qDebug() << "Setting Circle" << v->index() << "with radius:" << v->radius() << "at pos " << v->center();
        //and update the lists
        unplacedCircles.removeAll(v);
        placedCircles.append(v);
        //and then we continue
    }

}

qreal EuclidPacking::angle(qreal r, qreal ra, qreal rb) const
{
    qreal a = r + ra;
    qreal b = r + rb;
    qreal c = ra + rb;

    qreal arg = (a*a + b*b - c*c)/(2.0 * a * b);
    qreal angle = acos(arg);
    return angle;

}



EuclidPacking& EuclidPacking::operator=(EuclidPacking&& other)
{
    this->graph_ = std::move(other.graph_);
    this->circles_ = std::move(other.circles_);
    return *(this);
}



void EuclidPacking::spawnCircles()
{
    for(int i: this->graph_->getNodes()){
        this->circles_.insert(i, std::make_shared<EuclidCircle>(i) );
    }
}


bool Circles::Packing::operator==(const EuclidPacking& lhs, const EuclidPacking& rhs)
{
    //Packings are equal if their graphs are equivalent and their circles have the same dimensions.
    bool equalGraphs = (lhs.graph_ == rhs.graph_);
    auto ca = lhs.circles();
    auto cb = rhs.circles();
    bool equalCircles = (ca == cb);
    return equalGraphs && equalCircles;
}
