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
    this->circle = std::make_unique<Packing::EuclidCircle>();
    this->setPos(this->circle->projCenter());
}

Ui::GraphicCircle::GraphicCircle(const Packing::Circle& c):
    QGraphicsItem()
{
    this->circle = c.clone();
    this->setPos(this->circle->projCenter());
}

QRectF GraphicCircle::boundingRect() const
{
    qreal r = this->circle->projRadius();
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
    qreal radius = this->circle->projRadius();
    painter->setPen(QPen(Qt::black, 1.50/lod ));
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(QPointF(0, 0), radius, radius);


}

int GraphicCircle::graphIndex() const
{
    return this->circle->index();
}
