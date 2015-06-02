#include "circle.h"

#include <QTWidgets>

Circle::Circle(double x, double y, double r){
    this->x = x;
    this->y = y;
    this->r = r;
}

Circle::~Circle()
{

}

QRectF Circle::boundingRect() const {
    return QRectF(0, 0, this->r*2, this->r*2);
}

QPainterPath Circle::shape() const {
    QPainterPath path;
    path.addEllipse(0, 0, this->r*2, this->r*2);
    return path;
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    QColor fillColor(255, 255, 255);
    painter->setPen(QPen(Qt::black, 4));
    painter->setBrush(QBrush(fillColor));
    painter->drawEllipse(0, 0, this->r*2, this->r*2);
}

