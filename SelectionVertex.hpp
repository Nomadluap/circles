#ifndef SELECTIONVERTEX_HPP
#define SELECTIONVERTEX_HPP

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>

/**
 * Class which represents a selected vertex in the shape selector view.
 */
class SelectionVertex : public QGraphicsItem
{
public:
    static const qreal size; //size of the length of the bounding box
    static const qreal thickness; //thickness of the line

    SelectionVertex(QPointF position, QColor color = QColor(255, 0, 0));
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    QColor color;
};

#endif // SELECTIONVERTEX_HPP
