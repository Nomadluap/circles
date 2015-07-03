#include "Background.hpp"
#include "QtWidgets"
Background::Background()
{
    this->setPos(0, 0);
}

QRectF Background::boundingRect() const
{
    return QRectF(-1, -1, 2, 2);
}

void Background::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(QPen(Qt::green, 1.0/lod ));
    painter->drawEllipse(QPointF(0, 0), 1, 1);
}

