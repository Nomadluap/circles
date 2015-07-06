#include "Node.hpp"
#define PI 3.1415926535897932384626433

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

QList<Node *> Node::generateHexArray(int size, qreal radius)
{
    //first we must generate a 2d vector of the appropriate size
    QVector<QVector<Node*> > mat;
    mat.resize(size);
    //now fill it with nodes
    for(int row=0; row < size; row++){
        for(int col=0; col< size; col++){
            Node *n = new Node(row*size + col);
            n->setRadius(radius);
            qreal xpos = 2 * (row - size/2) * radius + 2 * (col - size/2)*radius*cos(2*PI/3.0);
            qreal ypos = 2 * (col - size/2) * radius * sin(2 * PI/3.0);
            n->setPosition(QPointF(xpos, ypos));
            mat[row].append(n);
        }
    }
    //now set up neibhour relations with those nodes.
    for(int row=0; row < size; row++){
        for(int col=0; col< size; col++){
            Node *n = mat[row][col];
            if(row >= 1){
                n->addNeibhour(mat[row-1][col]);
                if(col >= 1) n->addNeibhour(mat[row-1][col-1]);
            }
            if(col <= size-2) n->addNeibhour(mat[row][col+1]);
            if(col >= 1) n->addNeibhour(mat[row][col-1]);

            if(row <= size-2){
                n->addNeibhour(mat[row+1][col]);
                if(col <= size-2) n->addNeibhour(mat[row+1][col+1]);
            }
        }
    }
    //now add the nodes to a new packing
    QList<Node*>  l;
    for(QVector<Node*> row: mat){
        for(Node* n: row){
            l.append(n);
        }
    }
    return l;
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
    return this->neibhours.first()->isNeibhour(this->neibhours.last());
}


