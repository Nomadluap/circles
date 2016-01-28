#include "Edge.hpp"

using namespace Circles::Graph;

Circles::Graph::Edge::Edge():
    x(-1),
    y(-1)
{
    //do nothing here.
}

Circles::Graph::Edge::Edge(int x, int y)
{
    // lower order coordinate should alway be first.
    if(x < y){
        this->x = x;
        this->y = y;
    }
    else{
        this->x = y;
        this->y = x;
    }
}

Circles::Graph::Edge::Edge(const Edge& other)
{
    this->x = other.x;
    this->y = other.y;
}

Edge& Circles::Graph::Edge::operator=(const Edge& other)
{
    this->x = other.x;
    this->y = other.y;

    return *this;
}

int Circles::Graph::Edge::getX()
{
    return this->x;
}

int Circles::Graph::Edge::getY(){
    return this->y;
}

void Circles::Graph::Edge::setX(int x)
{
    // maintain ordering
    if(x > this->y){
        this->x = this->y;
        this->y = x;
    }
    else{
        this->x = x;
    }
}

void Circles::Graph::Edge::setY(int y)
{
    // maintain ordering
    if(y < this->x){
        this->y = this->x;
        this->x = y;
    }
    else{
        this->y = y;
    }
}

void Circles::Graph::Edge::set(int x, int y)
{
    // maintain ordering
    if(x < y){
        this->x = x;
        this->y = y;
    }
    else{
        this->y = x;
        this->x = y;
    }
}

bool Circles::Graph::operator==(const Edge& lhs, const Edge& rhs)
{
    return (lhs.x == rhs.x) && (lhs.x == rhs.x);
}
