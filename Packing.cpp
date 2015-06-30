#include "Packing.hpp"
#include <QWidget>
#include <QtDebug>
#include <cmath>

#define PI 3.1415926535897932384626433



Packing::Packing(PackingType type)
{
    this->type = type;
}

Packing::Packing(QList<Node *> nodes, PackingType type):
    Packing(type)
{
    for(Node* n: nodes) this->addNode_fast(n);
    this->recomputeConnectors();
}

Packing::~Packing()
{
    //delete connectors
    for(Connector* c: this->connectors){
        if(c != nullptr) delete c;
    }
    //delete circles
    for(Circle* c: this->circles){
        if(c != nullptr) delete c;
    }
    //Since nodes can pass around, we need to conserve them.
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
    this->addNode_fast(n);
    this->recomputeConnectors();
}

void Packing::addNode_fast(Node *n)
{
    if(!this->nodes.contains(n)) this->nodes.append(n);
    this->addCircle(n);
}


void Packing::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << "scene got mouse press event";
    QGraphicsScene::mousePressEvent(mouseEvent);
    this->update();
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

void Packing::repack(qreal epsilon, qreal outerRadius)
{
    //first set all radii
    for(Node* n: this->nodes){
        n->setRadius(outerRadius);
    }
    //compute a list of all inner nodes
    QList<Node*> interior;
    for(Node* n: this->nodes){
        if(this->isInterior(n)) interior.append(n);
    }
    //Repacking loop
    bool done = false;
    while(!done){
        //recompute radii
        for(Node* n: interior){
            qreal theta = this->anglesum(n);
            qreal delta = fabs(2*PI - theta);
            if(theta < 2*PI){
                n->setRadius(n->getRadius() * (1 - delta/2.0));
            }
            else{
                n->setRadius(n->getRadius() * (1 + delta/2.0));
            }
        }
        //check that all radii satisfy the epsilon-condition
        done = true;
        for(Node* n: interior){
            if(fabs(this->anglesum(n) - 2*PI) >= epsilon){
                done=false;
                break;
            }
        }
    }
}

void Packing::layout(int centerCircle)
{
    QList<Node*> L = this->nodes;
    //place the first circle
    for(auto n: L){
        if (n->getId() == centerCircle){
            n->setPosition(QPointF(0, 0));
            //place a neibhour on the positive x-axis
            //need to get position so that hyperbolic distance is equal to r1+r2...
            Node *m = n->getNeibhours().first();
            L.removeAll(n);
            break;
        }
        //TODO: special case for hyperbolic circles and euclidean circles.
    }

}

qreal Packing::angle(Node *r, Node *ra, Node *rb)

{
    if(this->type == PackingType::EuclideanPacking){
        return this->angle_euclidean(r, ra, rb);
    }
    else{
        return this->angle_hyperbolic(r, ra, rb);
    }
}

qreal Packing::angle_euclidean(Node *r, Node *ra, Node *rb)
{
    //TODO
    return 0;
}

qreal Packing::angle_hyperbolic(Node *r, Node *ra, Node *rb)
{
    qreal a = r->getRadius() + ra->getRadius();
    qreal b = r->getRadius() + rb->getRadius();
    qreal c = ra->getRadius() + rb->getRadius();

    qreal arg = (cosh(a)*cosh(b) - cosh(c))/(sinh(a)*sinh(b));
    qreal angle = acos(arg);
    return angle;
}

qreal Packing::anglesum(Node *r)
{
    r->sortNeibhours();
    qreal sum = 0;
    QList<Node*> nbhd = r->getNeibhours();
    for(int i = 0; i < nbhd.length()-1; i++){
        sum += this->angle(r, nbhd.at(i), nbhd.at(i+1));
    }
    //and the last angle
    sum += this->angle(r, nbhd.at(nbhd.length()-1), nbhd.at(0));

    return sum;
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

bool Packing::isInterior(Node *n)
{
    return !(this->boundaryNodes.contains(n)) && this->nodes.contains(n);
}

void Packing::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}
