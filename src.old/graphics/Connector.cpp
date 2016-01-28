#include "Connector.hpp"
#include <cmath>
#include <QtWidgets>

Connector::Connector(Node* n1, Node* n2)
{
    this->n1 = n1;
    this->n2 = n2;
    QPointF p1 = n1->getPosition();
    QPointF p2 = n2->getPosition();
    qreal x = (p2.x() + p1.x())/2.0;
    qreal y = (p2.y() + p1.y())/2.0;
    this->midpoint = QPointF(x, y);
    this->setPos(midpoint);
}

QRectF Connector::boundingRect() const
{
    QPointF p1 = n1->getPosition();
    QPointF p2 = n2->getPosition();
    p1 -= this->midpoint;
    p2 -= this->midpoint;
    qreal x0 = (p1.x() < p2.x())? p1.x() : p2.x();
    qreal y0 = (p1.y() < p2.y())? p1.y() : p2.y();
    qreal dx = fabs(p2.x() - p1.x());
    qreal dy = fabs(p2.y() - p1.y());
    return QRectF(x0, y0, dx, dy);
}

void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(QPen(Qt::red, 1/lod));

    QPointF p1 = n1->getPosition();
    QPointF p2 = n2->getPosition();
    p1 -= this->midpoint;
    p2 -= this->midpoint;

    painter->drawLine(p1, p2);
}

