#include "Text.hpp"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QMap>



Circles::View::Text::Text(QMap<int, std::shared_ptr<Packing::Circle> > circles, QRectF boundingrect, QGraphicsItem* parent):
    QGraphicsItem(parent),
    boundingRect_(boundingrect)
{
    this->circles = circles;
}


Circles::View::Text::~Text()
{

}

QRectF Circles::View::Text::boundingRect() const
{
    return this->boundingRect_;
}

void Circles::View::Text::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->save();
    QFont font("Times", 10);
    painter->setFont(font);
    painter->scale(1.0/lod, 1.0/lod);
    for(int i: this->circles.keys()){
        //find the center point of the circle
        QPointF center = this->circles.value(i)->projCenter();
        painter->drawText(center*lod, QString("%1").arg(i));
    }
    painter->restore();
}
