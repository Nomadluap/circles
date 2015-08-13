#include "Packing.hpp"
#include <QWidget>
#include <QtDebug>
#include <cmath>
#include <complex>
#include "Boundary.hpp"

#define PI 3.1415926535897932384626433



Packing::Packing(const Packing *p)
{
    //class field copy
    this->type = p->type;
    this->boundary = new Boundary;
    this->addItem(boundary);
    //deep copy nodes and preserve graph structure
    //make new nodes
    for(Node *n: p->nodes){
        addNode_fast(new Node(n));
    }
    //now reestablish the neibhour relations
    for(int i = 0; i < this->nodes.length(); i++){
        Node* o = p->nodes.at(i);
        for(Node* oo: o->getNeibhours()){
            this->nodes.at(i)->addNeibhour(this->nodes.at(p->nodes.indexOf(oo)));
        }
    }

    this->recomputeConnectors();

}

Packing::Packing(PackingType type)
{
    this->type = type;
    this->boundary = new Boundary();
    this->addItem(boundary);
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
    if(!this->nodes.contains(n)){
        this->nodes.append(n);
        this->addCircle(n);
    }
}


void Packing::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << "scene got mouse press event";
    //update the property window
    QPointF pos = mouseEvent->scenePos();
    QList<QGraphicsItem*> items = this->items(pos);
    //cycle through the items to find one which is a circle.
    Node *n = nullptr;
    Circle *c = nullptr;
    for(auto item: items){
        c = dynamic_cast<Circle*>(item);
        if(c == nullptr) continue;
        else{
            n = c->getNode();
            break;
        }
    }
    if(n != nullptr) emit newNodeSelected(n);
    //handle circle visual selection
    if(c != nullptr){
        if(c == this->selectedCircle){
            c->setSelectionState(Circle::SelectionState::None);
            this->selectedCircle = nullptr;
        }
        else{
            if(this->selectedCircle != nullptr){
                this->selectedCircle->setSelectionState(Circle::SelectionState::None);
            }
            c->setSelectionState(Circle::SelectionState::Selected);
            this->selectedCircle = c;
        }
    }

    //QGraphicsScene::mousePressEvent(mouseEvent);
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

void Packing::setDrawBoundary(bool d)
{
    this->boundary->setVisible(d);
}

void Packing::repack(qreal epsilon, qreal outerRadius)
{
    qDebug() << "STARTING REPACK";
    //first set all radii
    for(Node* n: this->nodes){
        n->setRadius(outerRadius);
        qDebug() << "Setting node #" << n->getId() << "to radius" << outerRadius;
    }
    //compute a list of all inner nodes
    QList<Node*> interior;
    for(Node* n: this->nodes){
        if(this->isInterior(n)){
            interior.append(n);
            qDebug() << "Node" << n->getId() << "is interior";
        }
    }
    //Repacking loop
    bool done = false;
    while(!done){
        //recompute radii
        for(Node* n: interior){
            qDebug() << "\n for node:" << n->getId();
            qDebug() << "Old radius:" << n->getRadius();
            qreal theta = this->anglesum(n);
            qDebug() << "angle sum:" << theta;
            /* --OLD THETA CODE --
            qreal delta = fabs(2*PI - theta);
            if(theta < 2*PI){
                n->setRadius(n->getRadius() * (1 - delta/3.0));
            }
            else{
                n->setRadius(n->getRadius() * (1 + delta/3.0));
            }
            */
            //NEW CODE FROM PRACTICUM 3 PAGE 243
            int k = n->getNeibhourCount();
            qreal r = n->getRadius();
            qreal beta = sin(theta / (2.0 * k));
            qreal delta = sin(PI / qreal(k));
            qreal rhat = (beta / (1.0 - beta)) * r;
            qreal p = ((1.0 - delta) / delta) * rhat;
            n->setRadius(p);
            qDebug() << "New radius:" << n->getRadius();

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

    qDebug () << "DONE TEH REPACK";
}

void Packing::layout(int centerCircle)
{

    if(this->type == PackingType::EuclideanPacking)
        this->layout_euclidean(centerCircle);
    else if(this->type == PackingType::HyperbolicPacking)
        this->layout_hyperbolic(centerCircle);

    this->recomputeConnectors();
    this->update();

}

void Packing::layout_hyperbolic(int centerCircle)
{
    qDebug() << "Performing hyperbolic layout...";
    QList<Node*> unplacedNodes(this->nodes); //Nodes which have not yet been placed
    QList<Node*> placedNodes; //nodes which have been placed but do not have full flowers.
    QList<Node*> floweredNodes; //nodes for which their entire flower has been placed.
    //place the first circle
    bool foundCenterCircle = false;
    for(auto n: unplacedNodes){ //find the circle that is to be the center circle.
        if (n->getId() == centerCircle){
            qDebug() << "Placing first node #" << n->getId() << " at (0, 0)";
            n->setPosition(QPointF(0, 0));
            placedNodes.append(n);
            unplacedNodes.removeAll(n);
            //place the second node to right of the first node.
            Node *m = n->getNeibhours().first();

            if(!n->hasFullFlower()){
                int mindex = 1;
                do{
                    m = n->getNeibhours().at(mindex);
                    mindex++;
                } while(!m->hasFullFlower() && mindex < n->getNeibhours().length());

                if(mindex >= n->getNeibhours().length()){
                    qDebug() << "Neither M or N has full flower. Fail.";
                    return;
                }
            }
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
            foundCenterCircle = true;
            break;
        }
    }
    //fail if we didn't find the center circle
    if(!foundCenterCircle){
        qDebug() << "Could not find specified center circle. Fail.";
        return;
    }
    //now continue until all nodes have been placed...
    while(!unplacedNodes.empty()){
        qDebug() << "--NEXT NODE--";
        //find a placed node that does not have a full flower
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
            continue;
        }
        Node *u;
        if(nbhrIndex == 0) u = w->getNeibhours().last();
        else u = w->getNeibhours().at(nbhrIndex-1);
        //now v becomes this "first unplaced node"
        Node *v = w->getNeibhours().at(nbhrIndex);
        if(!unplacedNodes.contains(v)){
            qDebug() << "something went terribly wrong";
        }
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
        auto phiinv = [wp](QPointF zz)->QPointF{
            std::complex<double> c(wp.x(), wp.y());
            std::complex<double> cbar(wp.x(), -wp.y());
            std::complex<double> z(zz.x(), zz.y());

            std::complex<double> result = (z + c)/(1.0 + cbar*z);
            return QPointF(result.real(), result.imag());
        };
        qDebug() << "phi(u)=" << phi(u->getPosition());
        //find the angle <UWV=alpha
        qreal alpha = this->angle(w, u, v);
        qDebug() << "Calculated alpha " << alpha;
        QPointF relU = phi(u->getPosition());
        qreal beta = atan2(relU.y(), relU.x());
        qDebug() << "Calculated beta" << beta;

        //we need to determine if the nodes are currently being laid out in a
        //clockwise or anticlockwise manner. Thus we need to look at the two
        //previous unplaced nodes, and look at their relative angles.

        //this only works if w has two or more placed neibhours so far.
        int placedCount = 0;
        int isCCW = true;
        for(Node *n: placedNodes + floweredNodes){
            if(w->isNeibhour(n)) placedCount++;
        }
        if(placedCount >= 2 && w->getNeibhours().length() >= 3){
            //grab uprime
            Node *uprime;
            if(nbhrIndex == 0 && w->getNeibhours().length()){
                uprime = w->getNeibhours().at(w->getNeibhours().length() - 2);
            }
            else if(nbhrIndex == 1){
                uprime = w->getNeibhours().last();
            }
            else{
                uprime = w->getNeibhours().at(nbhrIndex - 2);
            }
            //now look at angles of uprime and u
            //QPointF relUPrime = uprime->getPosition() - w->getPosition();
            QPointF relUPrime = phi(uprime->getPosition());
            qreal betaprime = atan2(relUPrime.y(), relUPrime.x());
            //difference between angles should be less than PI radians
            qreal diff = fmod(betaprime - beta + 2*PI, 2*PI);
            if(diff < PI){
                //betaprime is "ahead" of beta, so we should continue clockwise
                isCCW = false;
                qDebug() << "Placing clockwise";
            }
            else{
                //betaprime is "behind" beta, so continue anticlockwise
                isCCW = true;
                qDebug() << "Placing Counterclockwise";
            }
        }

        qreal arg;
        if(isCCW) arg = fmod(beta+alpha+2*PI, 2*PI);
        else arg = fmod(beta-alpha+2*PI, 2 * PI);
        qDebug() << "Therefore arg(v)=" << arg;
        //now we plot the position of v, assuming that w is at the origin.
        //find euclidean distance s such that the hyperbolic distance from 0 to
        //s is equal to the sum of the hyperbolic radii.
        qreal r = w->getRadius() + v->getRadius();
        qreal s = (exp(r) - 1.0)/(exp(r)+1.0);
        qDebug() << "s=" << s;
        //now plot the point using sin and cosine
        QPointF pos(s*cos(arg), s*sin(arg));
        //this is teh position relative to w. Now for
        //set the position of v, remembering to take the isometry into account.
        qDebug() << "pos=" << pos;
        qDebug()  << "phiinv(pos)=" << phiinv(pos);
        QPointF position = phiinv(pos);
        //QPointF position = w->getPosition() + pos;
        v->setPosition(position);
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
    bool foundCenterCircle = false;
    for(auto n: unplacedNodes){ //find the circle that is to be the center circle.
        if (n->getId() == centerCircle){
            qDebug() << "Placing first node #" << n->getId() << " at (0, 0)";
            n->setPosition(QPointF(0, 0));
            placedNodes.append(n);
            unplacedNodes.removeAll(n);
            //place the second node to right of the first node.
            Node *m = n->getNeibhours().first();

            //at least one of m and n need to have a full flower
            if(!n->hasFullFlower()){
                int mindex = 1;
                do{
                    m = n->getNeibhours().at(mindex);
                    mindex++;
                } while(!m->hasFullFlower() && mindex < n->getNeibhours().length());
                //fail if we didn't find a proper m.
                if(mindex >= n->getNeibhours().length()){
                    qDebug() << "Neither M or N has full flower. Fail.";
                    return;
                }
            }

            qreal h1 = n->getRadius();
            qreal h2 = m->getRadius();

            qreal s = h1+h2;
            m->setPosition(QPointF(s, 0));
            qDebug() << "Placing second node #" << m->getId() << " at (" <<
                        s << ", 0)";
            placedNodes.append(m);
            unplacedNodes.removeAll(m);
            foundCenterCircle = true;
            break;
        }
    }
    //fail if we don't find the specified center circle.
    if(!foundCenterCircle){
        qDebug() << "Could not find specified center circle. Fail";
        return;
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
        } while(!w->hasFullFlower() && wIndex < placedNodes.length());
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
                fullFlower = true;
                break;
            }
        }
        //if the full flower is complete, then update the lists and try again
        //with a different node.
        if(fullFlower){
            qDebug() << "Node" << w->getId() << "has a full flower...";
            placedNodes.removeAll(w);
            floweredNodes.append(w);
            continue;
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

        //we need to determine if the nodes are currently being laid out in a
        //clockwise or anticlockwise manner. Thus we need to look at the two
        //previous unplaced nodes, and look at their relative angles.

        //this only works if w has two or more placed neibhours so far.
        int placedCount = 0;
        int isCCW = true;
        for(Node *n: placedNodes + floweredNodes){
            if(w->isNeibhour(n)) placedCount++;
        }
        if(placedCount >= 2 && w->getNeibhours().length() >= 3){
            //grab uprime
            Node *uprime;
            if(nbhrIndex == 0 && w->getNeibhours().length()){
                uprime = w->getNeibhours().at(w->getNeibhours().length() - 2);
            }
            else if(nbhrIndex == 1){
                uprime = w->getNeibhours().last();
            }
            else{
                uprime = w->getNeibhours().at(nbhrIndex - 2);
            }
            //now look at angles of uprime and u
            QPointF relUPrime = uprime->getPosition() - w->getPosition();
            qreal betaprime = atan2(relUPrime.y(), relUPrime.x());
            //difference between angles should be less than PI radians
            qreal diff = fmod(betaprime - beta + 2*PI, 2*PI);
            if(diff < PI){
                //betaprime is "ahead" of beta, so we should continue clockwise
                isCCW = false;
                qDebug() << "Placing clockwise";
            }
            else{
                //betaprime is "behind" beta, so continue anticlockwise
                isCCW = true;
                qDebug() << "Placing Counterclockwise";
            }
        }
        //then the actual argument of v is beta + alpha or beta - alpha
        qreal arg;
        if(isCCW) arg = fmod(beta+alpha+2*PI, 2*PI);
        else arg = fmod(beta-alpha+2*PI, 2 * PI);

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
        c->setVisible(false);
        this->removeItem(c);
        //if (c != nullptr) delete c;
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
    //return !(this->boundaryNodes.contains(n)) && this->nodes.contains(n);
    return this->nodes.contains(n) && n->hasFullFlower();
}

bool Packing::isExterior(Node *n)
{
    return this->nodes.contains(n) && !n->hasFullFlower();
}

void Packing::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}
