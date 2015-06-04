#include "EuclideanCircle.hpp"
#include <QtWidgets>

EuclideanCircle::EuclideanCircle(Node *n, Packing *p):
    Circle(n, p)
{

}

QRectF EuclideanCircle::boundingRect() const
{
    qreal radius = this->node->getRadius();
    return QRectF(-radius, -radius, 2*radius, 2*radius);
}

void EuclideanCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    //get the scale of the scene
    qreal lod = option->levelOfDetail;
    qreal radius = this->node->getRadius();
    if(parent->getDrawCircles()){
        painter->setPen(QPen(Qt::black, BORDER_THICKNESS*lod ));
        painter->setBrush(QBrush(this->node->getColor()));
        painter->drawEllipse(QPointF(0, 0), radius, radius);
    }
    if(parent->getDrawCenters()){
        painter->setPen(QPen(Qt::black, 1));
        painter->drawLine(QPointF(0, radius/4.0), QPointF(0, -radius/4.0));
        painter->drawLine(QPointF(radius/4.0, 0), QPointF(-radius/4.0, 0));
    }
    if(parent->getDrawIndicies()){
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->drawText(QPointF(2, -2), QString("%1").arg(this->node->getId()));
        painter->restore();
    }
}

