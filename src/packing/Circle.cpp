#include "Circle.hpp"

using namespace Circles::Packing;


Circles::Packing::Circle::Circle():
    _index(-1),
    _radius(1.0),
    _center(0, 0)
{

}

Circle::Circle(const QPointF& center, qreal radius, int index):
    _index(index),
    _radius(radius),
    _center(center)
{

}

Circle::Circle(const Circle& other):
    _index(other._index),
    _radius(other._radius),
    _center(other._center)
{

}

Circle::Circle(Circle&& other):
    _index(other._index),
    _radius(other._radius),
    _center(other._center)
{

}

Circle&Circle::operator=(const Circle& other)
{
    this->_index = other._index;
    this->_center = other._center;
    this->_radius = other._radius;

    return *this;
}

Circle& Circle::operator=(Circle&& other)
{
    this->_index = other._index;
    this->_center = other._center;
    this->_radius = other._radius;

    return *this;
}

int Circles::Packing::Circle::index() const
{
    return this->_index;
}

void Circles::Packing::Circle::setIndex(int index)
{
    this->_index = index;
}


bool Circles::Packing::operator<(const Circle &lhs, const Circle &rhs)
{
    return lhs._index < rhs._index;
}
