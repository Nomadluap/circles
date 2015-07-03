#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <QWidget>
#include <QGraphicsItem>

class Background : public QGraphicsItem
{
public:
    Background();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) Q_DECL_OVERRIDE;
};

#endif // BACKGROUND_HPP
