#include "Text.hpp"
#include <QPainter>
#include <QStyleOptionGraphicsItem>


Circles::View::Text::Text(QGraphicsItem* parent):
    QGraphicsSimpleTextItem(parent)
{

}

Circles::View::Text::Text(const QString& text, QGraphicsItem* parent):
    QGraphicsSimpleTextItem(text, parent)
{

}

Circles::View::Text::~Text()
{

}

void Circles::View::Text::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    painter->scale(1.0/lod, 1.0/lod);
    QGraphicsSimpleTextItem::paint(painter, option, widget);
}
