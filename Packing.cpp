#include "Packing.hpp"
#include <QWidget>
#include <QtDebug>
#include <cmath>
#include <complex>


#define PI 3.1415926535897932384626433



Packing::Packing(PackingType type)
{
    this->type = type;
//    this->boundary = new Boundary();
//    if(this->boundary != nullptr) this->addItem(boundary);
}

Packing::Packing(QList<Node *> nodes, PackingType type):
    Packing(type)
{
    for(Node* n: nodes) this->addNode_fast(n);
    this->recomputeConnectors();
}

Packing::~Packing()
{
    if(boundary!=nullptr) delete boundary;
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
    if(this->type == PackingType::EuclideanPacking)
        this->layout_euclidean(centerCircle);
    else if(this->type == PackingType::HyperbolicPacking)
        this->layout_hyperbolic(centerCircle);

    this->purgeCircles();
    for(Node* n: this->nodes){
        this->addCircle(n);
    }
    this->recomputeConnectors();

}

void Packing::layout_hyperbolic(int centerCircle)
{
    QList<Node*> availNodes(this->nodes);
    QList<Node*> usedNodes;
    //place the first circle
    for(auto n: availNodes){
        if (n->getId() == centerCircle){
            n->setPosition(QPointF(0, 0));
            usedNodes.append(n);
            availNodes.removeAll(n);

            Node *m = n->getNeibhours().first();
            qreal h1 = n->getRadius();
            qreal h2 = m->getRadius();
            //using the inverse of the log-formula since the first point is at the origin.
            qreal s = (exp(h1 + h2) - 1)/(exp(h1 + h2) + 1);
            m->setPosition(QPointF(s, 0));
            usedNodes.append(m);
            availNodes.removeAll(m);
            break;
        }
    }
    //until we've placed all nodes.
    while(availNodes.empty() == false){
        for(Node* v: availNodes){
            //find two neibhours who are neibhours of eachother
            Node* u = nullptr;
            Node* w = nullptr;
            for(int i = 0; i < usedNodes.length()-1; i++){
                Node* uu = usedNodes.at(i);
                Node* ww;
                if(!v->isNeibhour(uu)) continue;
                for(int j = i+1; j < usedNodes.length(); j++){
                    ww = usedNodes.at(j);
                    if(!v->isNeibhour(ww)) continue;
                    if(uu->isNeibhour(ww)) break;
                }
                if(uu->isNeibhour(ww)){ //we found a pair
                    u = uu;
                    w = ww;
                    break;
                }
            }
            if(u == nullptr || v == nullptr) continue;
            //now we need to compute the position of v given u and w.
            qreal ru = u->getRadius();
            qreal rv = v->getRadius();
            qreal rw = w->getRadius();
            qreal a = ru + rw;
            qreal b = rv + rw;
            qreal c = ru + rv;

            qreal arg = (cosh(a) * cosh(b) - cosh(c))/(sinh(a) * sinh(b));
            qreal alpha = acos(arg);

            //create a lambda for the isometry phi_w
            QPointF wp = w->getPosition();
            auto phi = [wp](QPointF zz)->QPointF{
                std::complex<double> c(wp.x(), wp.y());
                std::complex<double> cbar(wp.x(), -wp.y());
                std::complex<double> z(zz.x(), zz.y());

                std::complex<double> result = (z - c)/(cbar*z - 1.0);
                return QPointF(result.real(), result.imag());
            };
            //now we can apply the isometry
            QPointF uprime = phi(u->getPosition());
            qreal beta = atan2(uprime.y(), uprime.x());

            qreal s = (exp(rw+rv) - 1)/(exp(rw+rv) + 1);
            qreal x = -s*cos(alpha-beta);
            qreal y = s*sin(alpha-beta);
            QPointF vprime(x, y);
            QPointF vpos = phi(vprime);

            v->setPosition(vpos);
            //and now v has a position, so we can move it to the other list.
            availNodes.removeAll(v);
            usedNodes.append(v);
        }
    }

}

void Packing::layout_euclidean(int centerCircle)
{
    //TODO
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

void Packing::purgeCircles()
{
    for(Circle* c: this->circles){
        this->removeItem(c);
    }
    this->circles.clear();
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

QList<Node *> Packing::getNodes()
{
    return QList<Node*>(this->nodes);
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
