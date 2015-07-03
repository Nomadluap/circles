#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

class Connector;

#include <QWidget>
#include <QGraphicsItem>
#include "Node.hpp"

class Connector : public QGraphicsItem
{
public:
    Connector(Node* n1, Node* n2);
    QRectF	boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) Q_DECL_OVERRIDE;

private:
    Node *n1;
    Node *n2;
    QPointF midpoint;
};

#endif // CONNECTOR_HPP
