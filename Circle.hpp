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
    enum class SelectionState{None, Selected, Surrounded};

    Circle(Node *n, Packing *p);

    QRectF	boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) Q_DECL_OVERRIDE;

    SelectionState getSelectionState();
    void setSelectionState(SelectionState s);

    Node* getNode();

private:

    void mousePressEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;

    QRectF boundingRect_euclidean() const;
    QRectF boundingRect_hyperbolic() const;

    void paint_euclidean(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr);
    void paint_hyperbolic(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr);

    /**
     * @brief hyp_getEuclideanCenter
     * @return the location of the center of the euclidean circle which
     * corresponds to this hyperbolic circle
     */
    QPointF hyp_getEuclideanCenter();

    /**
     * @brief hyp_getEuclideanRadius
     * @return radius of euclidean circle corresponding to this hyperbolic
     * circle.
     */
    qreal hyp_getEuclideanRadius();

    QColor getSelectionColor();


    static const qreal BORDER_THICKNESS;

    Node *node;
    Packing *parent;
    SelectionState selectionState = SelectionState::None;
};

#endif // CIRCLE_HPP
