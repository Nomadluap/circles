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

    QRectF	boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) Q_DECL_OVERRIDE;

private:
    QRectF boundingRect_euclidean() const;
    QRectF boundingRect_hyperbolic() const;

    void paint_euclidean(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr);
    void paint_hyperbolic(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr);

    QPointF hyp_getActualCenter();
    qreal hyp_getActualRadius();

    static const qreal BORDER_THICKNESS;

    Node *node;
    Packing *parent;

};

#endif // CIRCLE_HPP
