#include "SelectionPacking.hpp"
#include <typeinfo>
#include <QtDebug>


SelectionPacking::SelectionPacking(PackingType type):
    Packing(type)
{

}

SelectionPacking::SelectionPacking(QList<Node *> nodes, PackingType type):
    Packing(nodes, type)
{

}

bool SelectionPacking::addToSelection(Circle *c)
{
    //if the selection list contains points, and the last circle's node is not
    //a neibhour to this circle's node, then we fail.
    if(this->selectedCircs.contains(c)) return false;
    else if(!this->selectedCircs.empty() &&
            !this->selectedCircs.back()->getNode()->isNeibhour(c->getNode())){
        return false;
    }
    else{
        this->selectedCircs.push_back(c);
        c->setSelectionState(Circle::SelectionState::Selected);
        return true;
    }
}

void SelectionPacking::removeFromSelection(Circle *c)
{
    //do nothing if selection area is empty
    if(this->selectedCircs.empty()) return;
    //at this point we will only remove a circle from the list if it is the last
    //or first circle in the selection list
    if(c == this->selectedCircs.first()){
        c->setSelectionState(Circle::SelectionState::None);
        this->selectedCircs.removeFirst();
    }
    else if (c == this->selectedCircs.last()){
        c->setSelectionState(Circle::SelectionState::None);
        this->selectedCircs.removeLast();
    }
}

bool SelectionPacking::isInSelection(Circle *c)
{
    return this->selectedCircs.contains(c);
}

void SelectionPacking::clearSelection()
{
    for(Circle* c: this->selectedCircs){
        c->setSelectionState(Circle::SelectionState::None);
    }
    this->selectedCircs.clear();
}

void SelectionPacking::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << "Scene got mouse press event";
    //deterimine which button is being pressed
    if(mouseEvent->button() == Qt::LeftButton){
        this->mouseMode = MouseMode::Add;
    }
    else if(mouseEvent->button() == Qt::RightButton){
        this->mouseMode = MouseMode::Subtract;
    }
}

void SelectionPacking::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();
    QList<QGraphicsItem*> g = this->items(pos);
    QList<Circle*> circs;

    //try casting intersecting objects so that we only get circles.
    for(QGraphicsItem* i: g){
        Circle* c = dynamic_cast<Circle*> (i);
        if(c != nullptr) circs.append(c);
    }
    //if we're removing objects from the selection
    if(this->mouseMode == MouseMode::Subtract){
        //all circle items under the mouse cursor get deselected.
        for(Circle* c: circs){
            this->removeFromSelection(c);
        }
    }
    //if we're adding, then add
    if(this->mouseMode == MouseMode::Add){
        //all circle items under the mouse cursor get seleceted
        for(Circle* c: circs){
            this->addToSelection(c);
        }
    }
}

void SelectionPacking::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Scene got mouse release event";
    this->mouseMode = MouseMode::None;
}
