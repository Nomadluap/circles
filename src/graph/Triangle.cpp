#include "graph/Triangle.hpp"


using namespace Circles;
using namespace Circles::Graph;

Triangle::Triangle():
    p1_(-1),
    p2_(-1),
    p3_(-1)
{

}

Triangle::Triangle(Node t1, Node t2, Node t3)
{
    //find the smallest first
    if(t1 <= t2 && t1 <= t3){
        this->p1_ = t1;
        this->p2_ = (t2 <= t3) ? t2 : t3;
        this->p3_ = (t2 >= t3) ? t2 : t3;
    }
    else if(t2 <= t1 && t2 <= t3){
        this->p1_ = t2;
        this->p2_ = (t1 <= t3) ? t1 : t3;
        this->p3_ = (t1 >= t3) ? t1 : t3;
    }
    else{ // t3 is smallest
        this->p1_ = t3;
        this->p2_ = (t2 <= t1) ? t2 : t1;
        this->p3_ = (t2 >= t1) ? t2 : t1;
    }

}

Triangle::Triangle(const Triangle& other):
    p1_(other.p1_),
    p2_(other.p2_),
    p3_(other.p3_)
{

}

Triangle&Triangle::operator=(const Triangle& other)
{
    this->p1_ = other.p1();
    this->p2_ = other.p2();
    this->p3_ = other.p3();

    return *this;
}

bool Triangle::operator==(const Triangle& rhs) const
{
    return (this->p1_ == rhs.p1_) && (this->p2_ == rhs.p2_) && (this->p3_ == rhs.p3_);
}

bool Triangle::operator<(const Triangle& rhs) const
{
    if(this->p1_ < rhs.p1_) return true;
    else if(this->p2_ < rhs.p2_) return true;
    else if(this->p3_ < rhs.p3_) return true;
    else return false;
}

Node Triangle::p1() const
{
    return this->p1_;
}

Node Triangle::p2() const
{
    return this->p2_;
}

Node Triangle::p3() const
{
    return this->p3_;
}
