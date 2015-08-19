#include "Node.hpp"
#include <cmath>

#define PI 3.1415926535897932384626433

Node::Node(const Node *n)
{
    this->id = n->id;
    this->bHasPosition = n->bHasPosition;
    this->color = n->color;
    this->radius = n->radius;
    this->position = n->position;
}

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

Node::~Node()
{

}

QList<Node *> Node::generateHexArray(const QRectF &area, qreal radius)
{
    QPointF startpos = area.topLeft();
    qreal width = area.width();
    qreal height = area.height();
    //calculate n and m; n is "width" and m is "height"
    int n = int(width / (2*radius) + 1);
    int m = int( (height / (2*radius) - 1) * 2 / sqrt(3) + 2)+1; //needs to be odd
    return Node::generateHexArray(startpos, n, m, radius);


}

QList<Node *> Node::generateHexArray(const QPointF &startpos, int w, int h, qreal radius)
{
    int n = w;
    int m = h;
    if(m % 2 == 0) m++; //ensure m is odd
    //number of nodes in packing
    int nodecount = (m+1)/2*n + (m-1)/2*(n-1);
    //generate node list
    QList<QList<int> > cplx;
    for(int i = 0; i < nodecount; i++){
        cplx.append(QList<int>());
    }
    //now generate nbhr relations
    //corners
    cplx[0] = {1, n};
    cplx[n-1] = {2*n-2, n-2};
    cplx[nodecount-1] = {nodecount-2, nodecount-n-1};
    cplx[nodecount-n] = {nodecount-2*n+1, nodecount-n+1};

    //top edge
    for(int i = 1; i < n-1; i++){
        cplx[i] = {i+1, i+n, i+n-1, i-1};
    }
    //bottom edge
    for(int i = nodecount-n+1; i < nodecount-1; i++){
        cplx[i] = {i-1, i-n, i-n+1, i+1};
    }
    //left edge even (wide) rows
    for(int i = 0; i < (m+1)/2 - 2; i++){
        int index = (i+1) * (2*n-1);
        cplx[index] = {index-n+1, index+1, index+n};
    }
    //right edge even(wide) rows
    for(int i = 0; i < (m+1)/2 - 2; i++){
        int index = (i+1) * (2*n-1) + n-1;
        cplx[index] = {index+n-1, index-1, index-n};
    }
    //left edge odd (short) rows
    for(int i = 0; i < (m+1)/2 - 1; i++){
        int index = n + i*(2*n-1);
        cplx[index] = {index-n, index-n+1, index+1, index+n, index+n-1};
    }
    //right edge odd (short) rows
    for(int i = 0; i < (m+1)/ 2 - 1; i++){
        int index = n + i*(2*n-1) + n-2;
        cplx[index] = {index+n, index+n-1, index-1, index-n, index-n+1};
    }
    //interior circles
    for(int i = 0; i < nodecount; i++){
        if(cplx[i].isEmpty()){
            cplx[i] = {i-n, i-n+1, i+1, i+n, i+n-1, i-1, i-n};
        }
    }
    //genereate nodes
    QList<Node*> nodes;
    for(int i = 0; i < nodecount; i++){
        Node* n = new Node(i);
        n->setRadius(radius);
        nodes.append(n);
    }
    //apply neibhour relations
    for(int i = 0; i < nodecount; i++){
        for(int j: cplx[i]){
            nodes[i]->addNeibhour(nodes[j]);
        }
    }

    //set positions
    for(int row = 0; row < m; row++){
        if(row % 2 == 0){ //even-numbered row
            //row/2 even rows and row/2 odd rows
            int base = (row/2) * n + (row/2) * (n-1);
            for(int col = 0; col < n; col++){
                QPointF pos = startpos + QPointF(2 * radius * col, sqrt(3) * row * radius);
                nodes[base+col]->setPosition(pos);
            }
        }
        else{ //odd numbered rows
            //one extra even row than before
            int base = ((row-1)/ 2 + 1) * n + ((row-1)/2) * (n-1);
            for(int col = 0; col < (n-1); col++){
                QPointF pos = startpos + QPointF(radius + 2*radius * col, sqrt(3) * row * radius);
                nodes[base+col]->setPosition(pos);
            }
        }
    }

    return nodes;
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
    //node cannot be a neibhour to itself
    if(node == this) return;
    //we must retain reflexivity
    if(!this->neibhours.contains(node)) this->neibhours.append(node);
    if(!node->neibhours.contains(this)) node->neibhours.append(this);
    this->sortedNeibhours = false;

}



void Node::delNeibhour(Node *node)
{
    if(this->neibhours.contains(node)) this->neibhours.removeAll(node);
    if(node->neibhours.contains(this)) node->neibhours.removeAll(this);
    this->sortedNeibhours = false;
}

void Node::purgeNeibhours()
{
    for(Node* n: this->neibhours) this->delNeibhour(n);
}

bool Node::isNeibhour(Node *node)
{
    return this->neibhours.contains(node) && node->neibhours.contains(this);
}

void Node::sortNeibhours()
{
    QList<Node*> unsorted = this->neibhours;
    QList<Node*> sorted;
    //add the first item
    sorted.append(unsorted.first());
    unsorted.removeFirst();
    while(!unsorted.empty()){
        for(Node *n: unsorted){
            //if n is a nbhr of the last sorted element, then place it at the
            //back of the sorted list. If it is a nbhr of the first element,
            //then place it at the front of the list.
            if(n->isNeibhour(sorted.back())){
                sorted.push_back(n);
                unsorted.removeOne(n);
            }
            else if(n->isNeibhour(sorted.front())){
                sorted.push_front(n);
                unsorted.removeOne(n);
            }
        }
    }
    this->neibhours = sorted;
    this->sortedNeibhours = true;
}

QList<Node *> Node::getNeibhours()
{
    return QList<Node*>(this->neibhours);
}

int Node::getNeibhourCount()
{
    return this->neibhours.length();
}

void Node::setPosition(QPointF &position)
{
    this->position = position;
    this->bHasPosition = true;

}

QPointF Node::getPosition()
{
    if(this->hasPosition()) return QPointF(this->position);
    else return QPointF();
}

bool Node::hasPosition(){
    return this->bHasPosition;
}

void Node::delPosition()
{
    this->bHasPosition = false;
}

bool Node::hasFullFlower()
{
    if(!this->sortedNeibhours) this->sortNeibhours();
    if(this->getNeibhourCount()<= 2) return false;
    return this->neibhours.first()->isNeibhour(this->neibhours.last());
}


