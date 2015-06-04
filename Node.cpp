#include "Node.hpp"

Node::Node(int id)
{
    this->id = id;
    this->bHasPosition = false;
    this->color = QColor(255, 255, 255);
    this->radius = 0.0;
}

Node::Node(int id, QPointF &position, qreal radius)
{
    this->id = id;
    this->position = position;
    this->radius = radius;
    this->bHasPosition = true;
    this->color = QColor(255, 255, 255);
}

int Node::getId()
{
    return this->id;
}


void Node::setId(int id)
{
    this->id = id;
}
qreal Node::getRadius()
{
    return this->radius;
}

void Node::setRadius(qreal radius)
{
    if(radius > 0.0) this->radius = radius;
    else this->radius = 0.0;
}
QColor Node::getColor()
{
    return color;
}

void Node::setColor(const QColor &color)
{
    this->color = color;
}

void Node::addNeibhour(Node *node)
{
    //we must retain reflexivity
    if(!this->neibhours.contains(node)) this->neibhours.append(node);
    if(!node->neibhours.contains(this)) node->neibhours.append(this);
}

void Node::delNeibhour(Node *node)
{
    if(this->neibhours.contains(node)) this->neibhours.removeAll(node);
    if(node->neibhours.contains(this)) node->neibhours.removeAll(this);
}

bool Node::isNeibhour(Node *node)
{
    return this->neibhours.contains(node) && node->neibhours.contains(this);
}

void Node::setPosition(QPointF &position)
{
    this->position = position;
    this->bHasPosition = true;

}

QPointF Node::getPosition()
{
    if(this->hasPosition()) return this->position;
    else return QPointF();
}

bool Node::hasPosition(){
    return this->bHasPosition;
}

void Node::delPosition()
{
    this->bHasPosition = false;
}

