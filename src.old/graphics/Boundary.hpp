#ifndef GRAPHICS_BOUNDARY_HPP
#define GRAPHICS_BOUNDARY_HPP

#include <QWidget>
#include <QGraphicsItem>

class Boundary : public QGraphicsItem
{
public:
    Boundary();
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
};

#endif // GRAPHICS_BOUNDARY_HPP
