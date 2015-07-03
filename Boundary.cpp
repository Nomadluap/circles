#include "Boundary.hpp"
#include "QtWidgets"
Boundary::Boundary()
{
    this->setPos(0, 0);
}

QRectF Boundary::boundingRect() const
{
    return QRectF(-1, -1, 2, 2);
}

void Boundary::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(QPen(Qt::green, 1.0/lod ));
    painter->drawEllipse(QPointF(0, 0), 1, 1);
}

