#include "EuclideanPacking.hpp"
#include "Circle.hpp"
#include "Packing.hpp"
#include "EuclideanCircle.hpp"
#include <QGraphicsScene>
#include <QWidget>

EuclideanPacking::EuclideanPacking()
{

}

void EuclideanPacking::addCircle(Node *n)
{
    Circle *c = new EuclideanCircle(n, this);
    circles.append(c);
    this->addItem(c);
}
