#include <cmath>
#include "HyperCircle.hpp"

using namespace Circles::Packing;

Circles::Packing::HyperCircle::HyperCircle():
    Circle()
{
    this->_index = -1;
    this->_center = QPointF(0, 0);
    this->_radius = 1.0;
}

Circles::Packing::HyperCircle::HyperCircle(int index):
    Circle(index)
{

}

Circles::Packing::HyperCircle::HyperCircle(const QPointF &center, qreal radius, int index):
    Circle(center, radius, index)
{
    this->_index = index;
    // if norm is >1 then we normalize.
    this->_center = center;
    if(this->_center.manhattanLength() > 1.0) this->_center /= this->_center.manhattanLength();

    this->_radius = radius;
}

Circles::Packing::HyperCircle::HyperCircle(const HyperCircle &other):
    Circle(other)
{
    this->_index = other._index;
    this->_center = other._center;
    this->_radius = other._radius;
}

HyperCircle& Circles::Packing::HyperCircle::operator =(const HyperCircle& other)
{
    this->_index = other._index;
    this->_center = other._center;
    this->_radius = other._radius;

    return *this;
}

QPointF Circles::Packing::HyperCircle::center() const
{
    return this->_center;
}

qreal Circles::Packing::HyperCircle::radius() const
{
    return this->_radius;
}

QPointF Circles::Packing::HyperCircle::projCenter() const
{
    qreal theta = atan2(this->_center.y(), this->_center.x());

    qreal c = sqrt(QPointF::dotProduct(this->_center, this->_center));
    qreal r = this->_radius;

    qreal s = (exp(r) - 1) / (exp(r) + 1);

    // now the isometry phi maps the real point c to the origin.
    //where phi(z) = (z-c)/(cz-1)
    qreal a = (s-c)/(s*c - 1);
    qreal b = (-s-c)/(-s*c - 1);

    //now magnitude of euclidean center is at midpoint of a and b
    qreal m = (a+b)/2.0;
    return QPointF(m*cos(theta), m*sin(theta));
}

qreal Circles::Packing::HyperCircle::projRadius() const
{
    qreal c = sqrt(QPointF::dotProduct(this->_center, this->_center));
    qreal r = this->_radius;

    qreal s = (exp(r) - 1) / (exp(r) + 1);

    // now the isometry phi maps the real point c to the origin.
    //where phi(z) = (z-c)/(cz-1)
    qreal a = (s-c)/(s*c - 1);
    qreal b = (-s-c)/(-s*c - 1);

    //now half of difference between a and b is the radius
    return fabs((a-b)/2.0);
}

void Circles::Packing::HyperCircle::setRadius(qreal r)
{
    this->_radius = r;
}

bool Circles::Packing::HyperCircle::setCenter(QPointF c)
{
    //we need to make sure the point lies inside the disc.
    if(c.manhattanLength() > 1.0) return false;
    else{
        this->_center = c;
        return true;
    }
}


bool Circles::Packing::operator ==(const HyperCircle& lhs, const HyperCircle& rhs)
{
    return (lhs._center == rhs._center) && (lhs._index == rhs._index) && (lhs._radius == rhs._radius);
}

