#include "GraphicCircle.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "packing/Circle.hpp"
#include "packing/EuclidCircle.hpp"
using namespace Circles;
using namespace Circles::Ui;
GraphicCircle::GraphicCircle():
    QGraphicsItem()
{
    this->setPos(0, 0);
    this->radius_ = 1.0;
    this->index_ = -1;
}

Ui::GraphicCircle::GraphicCircle(const Packing::Circle& c):
    QGraphicsItem()
{
    this->setPos(c.center());
    this->radius_ = c.radius();
    this->index_ = c.index();
}

QRectF GraphicCircle::boundingRect() const
{
    qreal r = this->radius_;
    return QRectF(-r, -r, r, r);
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
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(QPointF(0, 0), this->radius_, this->radius_);


}

int GraphicCircle::graphIndex() const
{
    return this->index_;
}
