#ifndef EUCLIDEANCIRCLE_HPP
#define EUCLIDEANCIRCLE_HPP

#include "Circle.hpp"
#include "Packing.hpp"
#include <QGraphicsItem>
#include <QWidget>

/**
 * @brief The EuclideanCircle class describes the euclidean circle define by a
 * node, used for painting on the surface.
 */
class EuclideanCircle : public Circle
{
public:
    EuclideanCircle(Node *n, Packing *p);
    QRectF	boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) Q_DECL_OVERRIDE;
};

#endif // EUCLIDEANCIRCLE_HPP
