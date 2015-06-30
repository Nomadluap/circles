#ifndef NODE_HPP
#define NODE_HPP

#include <QWidget>

/**
 * @brief The Node class represents the metadata of a circle in a circle packing.
 */
class Node
{
public:
    Node(int id);
    Node(int id, QPointF &position, qreal radius=0);
    ~Node();


    int getId();
    void setId(int value);

    qreal getRadius();
    void setRadius(const qreal value);

    QColor getColor();
    void setColor(const QColor &value);

    //neibhours must maintain reflexivity.
    void addNeibhour(Node *node);
    void delNeibhour(Node *node);
    bool isNeibhour(Node *node);
    void sortNeibhours();
    QList<Node*> getNeibhours();

    void setPosition(QPointF &position);
    QPointF getPosition();
    bool hasPosition();
    void delPosition();



protected:
    int id;
    QPointF position;
    qreal radius;
    QList<Node*> neibhours;
    QColor color;
    bool bHasPosition=false;
};

#endif // NODE_HPP
