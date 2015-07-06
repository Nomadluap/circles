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
    qDebug() << "Performing hyperbolic layout...";
    QList<Node*> unplacedNodes(this->nodes); //Nodes which have not yet been placed
    QList<Node*> placedNodes; //nodes which have been placed but do not have full flowers.
    QList<Node*> floweredNodes; //nodes for which their entire flower has been placed.
    //place the first circle
    for(auto n: unplacedNodes){ //find the circle that is to be the center circle.
        if (n->getId() == centerCircle){
            qDebug() << "Placing first node #" << n->getId() << " at (0, 0)";
            n->setPosition(QPointF(0, 0));
            placedNodes.append(n);
            unplacedNodes.removeAll(n);
            //place the second node to right of the first node.
            Node *m = n->getNeibhours().first();
            qreal h1 = n->getRadius();
            qreal h2 = m->getRadius();
            //using the inverse of the log-formula
            //since the first point is at the origin we don't have to use an
            //isometry.
            qreal s = (exp(h1 + h2) - 1)/(exp(h1 + h2) + 1);
            m->setPosition(QPointF(s, 0));
            qDebug() << "Placing second node #" << m->getId() << " at (" <<
                        s << ", 0)";
            placedNodes.append(m);
            unplacedNodes.removeAll(m);
            break;
        }
    }
    //now continue until all nodes have been placed...
    while(!unplacedNodes.empty()){
        qDebug() << "--NEXT NODE--";
        //find a placed node that does not have a full flower
        Node *w = placedNodes.first();
        qDebug() << "Center node: " << w->getId();
        //find a nbhr of w which has been placed.
        int nbhrIndex = 0;
        //not an infinite loop since every placed node has at least one placed neibhour.
        while(unplacedNodes.contains(w->getNeibhours().at(nbhrIndex))) nbhrIndex++;
        //now continue going around the nodes until we find the first one that is unplaced
        //we also need to check if the full flower has been placed.
        bool fullFlower = false;
        int nbhrOrigin = nbhrIndex;
        while(!unplacedNodes.contains(w->getNeibhours().at(nbhrIndex))){
            nbhrIndex = (nbhrIndex + 1) % w->getNeibhours().length();
            //if we wrap completely around, then we know that w's full flower
            //has been placed.
            if(nbhrIndex == nbhrOrigin){
                qDebug() << "Node " << w->getId() << "has a full flower";
                fullFlower = true;
                break;
            }
        }
        //if the full flower is complete, then update the lists and try again
        //with a different node.
        if(fullFlower){
            placedNodes.removeAll(w);
            floweredNodes.append(w);
        }
        Node *u;
        if(nbhrIndex == 0) u = w->getNeibhours().last();
        else u = w->getNeibhours().at(nbhrIndex-1);
        //now v becomes this "first unplaced node"
        Node *v = w->getNeibhours().at(nbhrIndex);
        qDebug() << "Node w has id " << w->getId() << ", radius " <<
                    w->getRadius() << " and position" << w->getPosition();
        qDebug() << "Node u has id " << u->getId() << ", radius " <<
                    u->getRadius() << " and position " << u->getPosition();
        qDebug() << "Node v has id " << v->getId() << " and radius " <<
                    v->getRadius();
        //now we create a lambda phi which is an isometry
        QPointF wp = w->getPosition();
        auto phi = [wp](QPointF zz)->QPointF{
            std::complex<double> c(wp.x(), wp.y());
            std::complex<double> cbar(wp.x(), -wp.y());
            std::complex<double> z(zz.x(), zz.y());

            std::complex<double> result = (z - c)/(1.0 - cbar*z);
            return QPointF(result.real(), result.imag());
        };
        qDebug() << "phi(w)=" << phi(w->getPosition());
        //find the angle <UWV=alpha
        qreal alpha = this->angle(w, u, v);
        qDebug() << "Calculated alpha " << alpha;
        //find the argument of u
        qreal beta = atan2(u->getPosition().y(), u->getPosition().x());
        qDebug() << "Calculated beta" << beta;
        //then the actual argument of v is alpha+beta
        qreal arg = fmod(alpha+beta, 2 * PI);
        qDebug() << "Therefore arg(v)=" << arg;
        //now we plot the position of v, assuming that w is at the origin.
        //find euclidean distance s such that the hyperbolic distance from 0 to
        //s is equal to the sum of the hyperbolic radii.
        qreal r = w->getRadius() + v->getRadius();
        qreal s = (exp(r) - 1.0)/(exp(r)+1.0);
        qDebug() << "s=" << s;
        //now plot the point using sin and cosine
        QPointF pos(s*cos(arg), s*sin(arg));
        //set the position of v, remembering to take the isometry into account.
        qDebug() << "pos=" << pos;
        qDebug()  << "phi(pos)=" << phi(pos);
        v->setPosition(phi(pos));
        //and update the lists
        unplacedNodes.removeAll(v);
        placedNodes.append(v);
        //and then we continue
    }
    qDebug() << "Layout complete";
}

void Packing::layout_euclidean(int centerCircle)
{
    qDebug() << "Performing euclidean layout...";
    QList<Node*> unplacedNodes(this->nodes); //Nodes which have not yet been placed
    QList<Node*> placedNodes; //nodes which have been placed but do not have full flowers.
    QList<Node*> floweredNodes; //nodes for which their entire flower has been placed.
    //place the first circle
    for(auto n: unplacedNodes){ //find the circle that is to be the center circle.
        if (n->getId() == centerCircle){
            qDebug() << "Placing first node #" << n->getId() << " at (0, 0)";
            n->setPosition(QPointF(0, 0));
            placedNodes.append(n);
            unplacedNodes.removeAll(n);
            //place the second node to right of the first node.
            Node *m = n->getNeibhours().first();
            qreal h1 = n->getRadius();
            qreal h2 = m->getRadius();

            qreal s = h1+h2;
            m->setPosition(QPointF(s, 0));
            qDebug() << "Placing second node #" << m->getId() << " at (" <<
                        s << ", 0)";
            placedNodes.append(m);
            unplacedNodes.removeAll(m);
            break;
        }
    }
    //now continue until all nodes have been placed...
    while(!unplacedNodes.empty()){
        qDebug() << "--NEXT NODE--";
        //find a placed node that does not have its full flower placed.
        //we require that the w node does in fact have a full flower.
        Node *w;
        int wIndex = 0;
        do{
            w = placedNodes.at(wIndex);
            wIndex++;
        } while(!w->hasFullFlower());
        qDebug() << "Center node: " << w->getId();
        //find a nbhr of w which has been placed.
        int nbhrIndex = 0;
        //not an infinite loop since every placed node has at least one placed neibhour.
        while(unplacedNodes.contains(w->getNeibhours().at(nbhrIndex))) nbhrIndex++;
        //now continue going around the nodes until we find the first one that is unplaced
        //we also need to check if the full flower has been placed.
        bool fullFlower = false;
        int nbhrOrigin = nbhrIndex;
        while(!unplacedNodes.contains(w->getNeibhours().at(nbhrIndex))){
            nbhrIndex = (nbhrIndex + 1) % w->getNeibhours().length();
            //if we wrap completely around, then we know that w's full flower
            //has been placed.
            if(nbhrIndex == nbhrOrigin){
                qDebug() << "Node " << w->getId() << "has a full flower";
                fullFlower = true;
                break;
            }
        }
        //if the full flower is complete, then update the lists and try again
        //with a different node.
        if(fullFlower){
            placedNodes.removeAll(w);
            floweredNodes.append(w);
        }
        Node *u;
        if(nbhrIndex == 0) u = w->getNeibhours().last();
        else u = w->getNeibhours().at(nbhrIndex-1);
        //now v becomes this "first unplaced node"
        Node *v = w->getNeibhours().at(nbhrIndex);
        qDebug() << "Node w has id " << w->getId() << ", radius " <<
                    w->getRadius() << " and position" << w->getPosition();
        qDebug() << "Node u has id " << u->getId() << ", radius " <<
                    u->getRadius() << " and position " << u->getPosition();
        qDebug() << "Node v has id " << v->getId() << " and radius " <<
                    v->getRadius();
        //find the angle <UWV=alpha
        qreal alpha = this->angle(w, u, v);
        qDebug() << "Calculated alpha " << alpha;
        //find the argument of u
        QPointF relU = u->getPosition() - w->getPosition();
        qreal beta = atan2(relU.y(), relU.x());
        qDebug() << "Calculated beta" << beta;
        //then the actual argument of v is alpha+beta
        qreal arg = fmod(alpha+beta, 2 * PI);
        qDebug() << "Therefore arg(v)=" << arg;

        qreal r = w->getRadius() + v->getRadius();
        qDebug() << "r=" << r;
        //now plot the point using sin and
        //remember that this point is an offset from w.
        QPointF pos(r*cos(arg), r*sin(arg));
        pos += w->getPosition();
        //set the position of v
        qDebug() << "pos=" << pos;
        v->setPosition(pos);
        //and update the lists
        unplacedNodes.removeAll(v);
        placedNodes.append(v);
        //and then we continue
    }
    qDebug() << "Layout complete";
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
    qreal a = r->getRadius() + ra->getRadius();
    qreal b = r->getRadius() + rb->getRadius();
    qreal c = ra->getRadius() + rb->getRadius();

    qreal arg = (a*a + b*b - c*c)/(2.0 * a * b);
    qreal angle = acos(arg);
    return angle;
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
    qDebug() << "clearing existing connectors";
    for(Connector *c: this->connectors){
        this->removeItem(c);
        if (c != nullptr) delete c;
    }
    this->connectors.clear();
    qDebug() << "drawing new connectors";
    if(this->getDrawLinks()){
        for(Node *n1: this->nodes){
            for (Node *n2: n1->getNeibhours()){
                if(n2->getId() < n1->getId()) continue;
                Connector *c = new Connector(n1, n2);
                connectors.append(c);
                this->addItem(c);
            }
        }
    }
    qDebug() << "Recomputing Connectors complete.";
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
