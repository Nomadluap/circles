#include "Packing.hpp"
#include "Circle.hpp"
#include <QtWidgets>
Circle::Circle(Node *n, Packing *p)
{
    this->node = n;
    this->parent = p;
    this->setPos(this->node->getPosition());
}

QRectF Circle::boundingRect() const
{
    if(this->parent->getType() == PackingType::EuclideanPacking){
        return this->boundingRect_euclidean();
    }
    else{
        return this->boundingRect_hyperbolic();
    }
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(this->parent->getType() == PackingType::EuclideanPacking){
        this->paint_euclidean(painter, option, widget);
    }
    else{
        this->paint_hyperbolic(painter, option, widget);
    }
}

QRectF Circle::boundingRect_euclidean() const
{
    qreal radius = this->node->getRadius();
    return QRectF(-radius, -radius, 2*radius, 2*radius);
}

QRectF Circle::boundingRect_hyperbolic() const
{
    return QRectF();
}

void Circle::paint_euclidean(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    //get the scale of the scene
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    qreal radius = this->node->getRadius();
    if(parent->getDrawCircles()){
        painter->setPen(QPen(Qt::black, BORDER_THICKNESS/lod ));
        painter->setBrush(QBrush(this->node->getColor()));
        painter->drawEllipse(QPointF(0, 0), radius, radius);
    }
    if(parent->getDrawCenters()){
        painter->setPen(QPen(Qt::black, 2/lod));
        painter->drawLine(QPointF(0, radius/4.0), QPointF(0, -radius/4.0));
        painter->drawLine(QPointF(radius/4.0, 0), QPointF(-radius/4.0, 0));
    }
    if(parent->getDrawIndicies()){
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(1.0/lod, 1.0/lod);
        painter->drawText(QPointF(4, -4), QString("%1").arg(this->node->getId()));
        painter->restore();
    }
}

void Circle::paint_hyperbolic(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}



const qreal Circle::BORDER_THICKNESS = 2.0;
