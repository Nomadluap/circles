#include "SelectionVertex.hpp"
#include <QPainter>
#include <QDebug>
#include <QStyleOptionGraphicsItem>

using namespace Circles::View;

const qreal SelectionVertex::size = 15;
const qreal SelectionVertex::thickness = 3;


SelectionVertex::SelectionVertex(QPointF position, QColor color)
{
    this->color = color;
    this->setPos(position);
}

QRectF SelectionVertex::boundingRect() const
{
    return QRectF(-size/2.0, -size/2.0, size, size);
}

void SelectionVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    qreal r = size/2.0;
    painter->scale(1/lod, 1/lod);
    painter->setPen(QPen(color, thickness));
    painter->drawLine(-r, -r, r, r);
    painter->drawLine(-r, r, r, -r);
    this->update();
}

