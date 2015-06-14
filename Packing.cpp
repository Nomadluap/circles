#include "Packing.hpp"
#include <QWidget>
#include <QtDebug>

Packing::Packing(PackingType type)
{
    this->type = type;
}

void Packing::setPackingType(PackingType type)
{
    this->type = type;
    //changing packing type forces complete redraw.
    this->clear();
    while(!circles.isEmpty()){
        circles.removeLast();
    }
    //and re-add circles for all nodes
    for(Node *n: this->nodes){
        this->addCircle(n);
    }
    this->recomputeConnectors();
    this->update();
}

PackingType Packing::getType()
{
    return this->type;
}

bool Packing::getDrawCenters()
{
    return this->drawCenters;
}

bool Packing::getDrawLinks()
{
    return this->drawLinks;
}

bool Packing::getDrawCircles()
{
    return this->drawCircles;
}

bool Packing::getDrawIndicies()
{
    return this->drawIndicies;
}

void Packing::addNode(Node *n){
    if(!this->nodes.contains(n)) this->nodes.append(n);
    this->addCircle(n);
    this->recomputeConnectors();
}

void Packing::addCircle(Node *n)
{
    Circle *c = new Circle(n, this);
    circles.append(c);
    this->addItem(c);

}

void Packing::recomputeConnectors()
{
    qDebug() << "recomputing Connectors...";
    for(Connector* c: this->connectors){
        if(c->scene() == this) this->removeItem(c);
        if (c != nullptr) delete c;
    }
    this->connectors.clear();

    if(this->getDrawLinks()){
        for(Node* n1: this->nodes){
            for (Node* n2: n1->getNeibhours()){
                if(n2->getId() < n1->getId()) continue;
                Connector* c = new Connector(n1, n2);
                connectors.append(c);
                this->addItem(c);
            }
        }
    }
}

void Packing::drawForeground(QPainter *painter, const QRectF &rect)
{

}

void Packing::setDrawCenters(bool d)
{

    this->drawCenters = d;
    this->update();
}

void Packing::setDrawLinks(bool d)
{
    this->drawLinks = d;
    this->recomputeConnectors();
    this->update();
}

void Packing::setDrawCircles(bool d)
{
    this->drawCircles = d;
    this->update();
}

void Packing::setDrawIndicies(bool d)
{
    this->drawIndicies = d;
    this->update();
}
