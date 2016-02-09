#include "PackingCoordinate.hpp"


using Circles::Graph::Node;
using Circles::Graph::Triangle;
using namespace Circles::Packing;

PackingCoordinate::PackingCoordinate():
    _t({0, 0, 0}),
    _a(0.0),
    _b(0.0),
    _c(0.0)
{

}

PackingCoordinate::PackingCoordinate(Circles::Graph::Triangle t, qreal a, qreal b, qreal c):
    _t(t),
    _a(a),
    _b(b),
    _c(c)
{

}

PackingCoordinate::PackingCoordinate(const PackingCoordinate& other):
    _t(other._t),
    _a(other._a),
    _b(other._b),
    _c(other._c)
{

}

PackingCoordinate& PackingCoordinate::operator=(const PackingCoordinate& other)
{
    this->_t = other._t;
    this->_a = other._a;
    this->_b = other._b;
    this->_c = other._c;
    return *this;
}

bool PackingCoordinate::operator==(const PackingCoordinate& other)
{
    bool t = (this->_t == other._t);
    bool abc = (this->_a == other._a && this->_b == other._b && this->_c == other._c);
    return t && abc;
}

qreal PackingCoordinate::a() const
{
    return this->_a;
}

qreal PackingCoordinate::b() const
{
   return this->_b;
}

qreal PackingCoordinate::c() const
{
    return this->_c;
}

const Circles::Graph::Triangle& PackingCoordinate::triangle() const
{
    return std::move(Triangle(this->_t));
}


