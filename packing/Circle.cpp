#include "Circle.hpp"

using namespace Circles::Packing;


Circles::Packing::Circle::Circle()
{

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
