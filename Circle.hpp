#ifndef CIRCLE_HPP
#define CIRCLE_HPP

class Circle;
#include <QGraphicsItem>
#include <QWidget>
#include "Node.hpp"
#include "Packing.hpp"

/**
 * @brief The Circle class represents an abstract circle. A circle may exist in
 * either a hyperbolic or euclidean geometry.
 */
class Circle : public QGraphicsItem
{
public:
    Circle(Node *n, Packing *p);

//    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) Q_DECL_OVERRIDE;
//    virtual QRectF	boundingRect() const Q_DECL_OVERRIDE;

    static const qreal BORDER_THICKNESS;

protected:
    Node *node;
    Packing *parent;

};

#endif // CIRCLE_HPP
