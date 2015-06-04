#include "Circle.hpp"
#include <QtWidgets>
Circle::Circle(Node *n, Packing *p)
{
    this->node = n;
    this->parent = p;
    this->setPos(this->node->getPosition());
}



const qreal Circle::BORDER_THICKNESS = 2.0;
