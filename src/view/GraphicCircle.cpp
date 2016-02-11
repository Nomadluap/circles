#include "GraphicCircle.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "packing/Circle.hpp"
#include "packing/EuclidCircle.hpp"
using namespace Circles;
using namespace Circles::View;
GraphicCircle::GraphicCircle():
    QGraphicsItem()
{
    this->setPos(0, 0);
    this->radius_ = 1.0;
    this->index_ = -1;
    this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    prepareGeometryChange();
}

View::GraphicCircle::GraphicCircle(const Packing::Circle& c):
    QGraphicsItem()
{
    this->setPos(c.projCenter());
    this->radius_ = c.projRadius();
    this->index_ = c.index();
}

QRectF GraphicCircle::boundingRect() const
{
    qreal r = this->radius_;
    return QRectF(-r, -r, 2*r, 2*r);
}

QPainterPath GraphicCircle::shape() const
{
    QPainterPath p;
    p.addEllipse(this->boundingRect());
    return std::move(p);
}

void GraphicCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget)
    //get the scale of the scene
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(QPen(Qt::black, 1.50/lod ));
    painter->setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter->setClipRect(option->exposedRect);
    painter->drawEllipse(QPointF(0, 0), this->radius_, this->radius_);

    QFont font("Times", 10);
    font.setStyleStrategy(QFont::ForceOutline);
    painter->setFont(font);
    painter->save();
    painter->scale(1.0/lod, 1.0/lod);
    painter->drawText(QPointF(0, 0), QString("%1").arg(this->index_));
    painter->restore();


}

int GraphicCircle::graphIndex() const
{
    return this->index_;
}
