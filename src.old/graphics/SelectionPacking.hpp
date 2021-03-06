#ifndef GRAPHICS_SELECTIONPACKING_HPP
#define GRAPHICS_SELECTIONPACKING_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "Packing.hpp"
#include "Circle.hpp"

class SelectionPacking : public Packing
{
public:
    SelectionPacking(PackingType type = PackingType::EuclideanPacking);
    SelectionPacking(QList<Node*> nodes, PackingType type = PackingType::HyperbolicPacking);

    /**
     * @brief Attempts to add a circle c to the collection of selected circles
     * If a previous circle is in the selection and the new circle is not
     * ajacent to that circle, then the circle will not be added.
     * @param c
     * @return
     */
    bool addToSelection(Circle* c);
    void removeFromSelection(Circle* c);
    bool isInSelection(Circle* c);
    void clearSelection();

    //mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    enum class MouseMode{None, Add, Subtract};

private:
    QList<Circle*> selectedCircs;
    MouseMode mouseMode = MouseMode::None;

};

#endif // GRAPHICS_SELECTIONPACKING_HPP
