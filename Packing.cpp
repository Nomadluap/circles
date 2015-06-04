#include "Packing.hpp"
#include "EuclideanCircle.hpp"
#include <QWidget>

Packing::Packing(PackingType type)
{
    this->type = type;
}

void Packing::setPackingType(Packing::PackingType type)
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
    this->update();
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
}

void Packing::addCircle(Node *n)
{
    if(this->type == PackingType::EuclideanPacking){
        Circle *c = new EuclideanCircle(n, this);
        circles.append(c);
        this->addItem(c);
    }
    //todo: add hyperbolic circles
}

void Packing::setDrawCenters(bool d)
{

    this->drawCenters = d;
    this->update();
}

void Packing::setDrawLinks(bool d)
{
    this->drawLinks = d;
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
