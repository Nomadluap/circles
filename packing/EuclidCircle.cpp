#include "EuclidCircle.hpp"
using namespace Circles::Packing;

Circles::Packing::EuclidCircle::EuclidCircle()
{
    this->_index = -1;
    this->_center = QPointF(0, 0);
    this->_radius = 1.0;
}

Circles::Packing::EuclidCircle::EuclidCircle(const QPointF &center, qreal radius, int index)
{
    this->_index = index;
    this->_center = center;
    this->_radius = radius;
}

Circles::Packing::EuclidCircle::EuclidCircle(const EuclidCircle &other)
{
    this->_index = other._index;
    this->_center = other._center;
    this->_radius = other._radius;
}

EuclidCircle& Circles::Packing::EuclidCircle::operator =(const EuclidCircle& other)
{
    this->_index = other._index;
    this->_center = other._center;
    this->_radius = other._radius;

    return *this;
}

QPointF Circles::Packing::EuclidCircle::center() const
{
    return this->_center;
}

qreal Circles::Packing::EuclidCircle::radius() const
{
    return this->_radius;
}

QPointF Circles::Packing::EuclidCircle::projCenter() const
{
    return this->_center;
}

qreal Circles::Packing::EuclidCircle::projRadius() const
{
    return this->_radius;
}

void Circles::Packing::EuclidCircle::setRadius(qreal r)
{
    this->_radius = r;
}

bool Circles::Packing::EuclidCircle::setCenter(QPointF c)
{
    // no restrictions since the euclidean plane is unbounded.
    this->_center = c;
    return true;
}


bool Circles::Packing::operator ==(const EuclidCircle& lhs, const EuclidCircle& rhs)
{
    return (lhs._center == rhs._center) && (lhs._index == rhs._index) && (lhs._radius == rhs._radius);
}

