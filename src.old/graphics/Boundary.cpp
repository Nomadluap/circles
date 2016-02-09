#include "Boundary.hpp"
#include "QtWidgets"
#include <cmath>
#define PI 3.141592654

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
    Q_UNUSED(option);
    Q_UNUSED(widget);
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(QPen(Qt::green, 1.0/lod ));

    painter->drawEllipse(QPointF(0, 0), 1, 1);

    //painter->setBrush(QBrush(Qt::green));

    for(int i = 0; i < 4; i++){
        QPointF center(cos(PI / 4.0 + i * PI / 2.0), sin(PI / 4.0 + i * PI / 2.0));
        painter->drawEllipse(center, 0.05, 0.05);
        painter->drawLine(center - QPointF(0, 0.05), center + QPointF(0, 0.05));
        painter->drawLine(center - QPointF(0.05,0), center + QPointF(0.05,0));
    }
}
