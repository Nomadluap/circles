#include "HyperPacking.hpp"
#include <QDebug>
#include <complex>

#include "HyperCircle.hpp"
using namespace Circles;
using namespace Circles::Packing;

const qreal PI = 3.141592653589793238462643383279502884;

Circles::Packing::HyperPacking::HyperPacking()
{
    this->graph_ = std::make_shared<Graph::Graph>();
}

HyperPacking::HyperPacking(std::shared_ptr<Graph::Graph> g)
{
    this->graph_ = g;
    this->spawnCircles();
}
HyperPacking::HyperPacking(const HyperPacking& other)
{
    this->graph_ = other.graph_;
    this->circles_ = other.circles_;
}

HyperPacking::HyperPacking(HyperPacking&& other)
{
    this->graph_ = std::move(other.graph_);
    this->circles_ = std::move(other.circles_);
}

HyperPacking& HyperPacking::operator=(const HyperPacking& other)
{
    this->graph_ = other.graph_;
    this->circles_ = other.circles_;
    return *(this);
}

void HyperPacking::layout()
{
    //fixme: clean up this code and move it into multiple functions?
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

            qreal s = (exp(h1 + h2) - 1)/(exp(h1 + h2) + 1);
            QPointF dcenter(s * cos(this->firstNeighbourAngle_), s * sin(this->firstNeighbourAngle_) );
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

        /*=========================================
         * Isometry for the hyperbolic layout is here
         * ========================================*/
        QPointF wp = w->center();

        auto phi = [wp](QPointF zz)->QPointF{
            std::complex<double> c(wp.x(), wp.y());
            std::complex<double> cbar(wp.x(), -wp.y());
            std::complex<double> z(zz.x(), zz.y());

            std::complex<double> result = (z - c)/(1.0 - cbar*z);
            return QPointF(result.real(), result.imag());
        };
        auto phiinv = [wp](QPointF zz)->QPointF{
            std::complex<double> c(wp.x(), wp.y());
            std::complex<double> cbar(wp.x(), -wp.y());
            std::complex<double> z(zz.x(), zz.y());

            std::complex<double> result = (z + c)/(1.0 + cbar*z);
            return QPointF(result.real(), result.imag());
        };


        //find the angle <UWV=alpha
        qreal alpha = this->angle(w->radius(), u->radius(), v->radius());
        //find the argument of u
        QPointF relU = phi(u->center());
        qreal beta = atan2(relU.y(), relU.x());

        //we need to determine if the nodes are currently being laid out in a
        //clockwise or anticlockwise manner. Thus we need to look at the two
        //previous unplaced nodes, and look at their relative angles.

        //this only works if w has two or more placed neibhours so far.
        int placedCount = 0;
        int isCCW = true;
        for(auto n: placedCircles + floweredCircles){
            if(neighbours(*w).contains(n)) placedCount++;
        } // for
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
            QPointF relUPrime = phi(uprime->center());
            qreal betaprime = atan2(relUPrime.y(), relUPrime.x());
            //difference between angles should be less than PI radians
            qreal diff = fmod(betaprime - beta + 2*PI, 2*PI);
            if(diff < PI){
                //betaprime is "ahead" of beta, so we should continue clockwise
                isCCW = false;
            } // if
            else{
                //betaprime is "behind" beta, so continue anticlockwise
                isCCW = true;
            } //  else
        } //  if placedcount
        //then the actual argument of v is beta + alpha or beta - alpha
        qreal arg;
        if(isCCW)   arg = fmod(beta+alpha+2*PI, 2 * PI);
        else        arg = fmod(beta-alpha+2*PI, 2 * PI);
        qDebug() << "circle" << v->index() << "arg is" << arg;
        qreal r = w->radius() + v->radius();
        qreal s = (exp(r) - 1.0)/(exp(r) + 1.0);
        //now plot the point using sin and
        //remember that this point is an offset from w.
        QPointF pos(s*cos(arg), s*sin(arg));
        qDebug() << "pre-phiinv" << pos;
        //set the position of v, remembering to take isometry into account.
        QPointF position = phiinv(pos);
        //set the position of v
        v->setCenter(position);
        qDebug() << "circle" << v->index() << "got position" << position;
        qDebug() << "Setting Circle" << v->index() << "with radius:" << v->radius() << "at pos " << v->center();
        //and update the lists
        unplacedCircles.removeAll(v);
        placedCircles.append(v);
        //and then we continue
    }

}

qreal HyperPacking::angle(qreal r, qreal ra, qreal rb) const
{
    qreal a = r + ra;
    qreal b = r + rb;
    qreal c = ra + rb;

    qreal arg = (cosh(a)*cosh(b) - cosh(c))/(sinh(a)*sinh(b));
    qreal angle = acos(arg);
    return angle;

}



HyperPacking& HyperPacking::operator=(HyperPacking&& other)
{
    this->graph_ = std::move(other.graph_);
    this->circles_ = std::move(other.circles_);
    return *(this);
}



void HyperPacking::spawnCircles()
{
    for(int i: this->graph_->getNodes()){
        this->circles_.insert(i, std::make_shared<HyperCircle>(i) );
    }
}


bool Circles::Packing::operator==(const HyperPacking& lhs, const HyperPacking& rhs)
{
    //Packings are equal if their graphs are equivalent and their circles have the same dimensions.
    bool equalGraphs = (lhs.graph_ == rhs.graph_);
    auto ca = lhs.circles();
    auto cb = rhs.circles();
    bool equalCircles = (ca == cb);
    return equalGraphs && equalCircles;
}
