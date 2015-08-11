#ifndef NODE_HPP
#define NODE_HPP

#include <QWidget>

/**
 * @brief The Node class represents the metadata of a circle in a circle packing.
 */
class Node
{
public:
    /**
     * Copy constructor. Note that neibhour relationships are not copied.
     * @param n Node to copy
     */
    Node(const Node *n);
    /**
     * Construct a Node without position or radius.
     * @param id unique id of the node
     */
    Node(int id);
    /**
     * Construct a node with a given position and radius.
     * @param id Unique id of the node
     * @param position Position of the node
     * @param radius radius of the node
     */
    Node(int id, QPointF &position, qreal radius=0);
    ~Node();

    /**
     * @brief Generates a list of nodes which form a specified hex-tiling,
     * suitable for passing to a Selection packing, with arbitrary shapes to
     *  begin a hyperbolic repacking
     * @param size the number of circles across to generate
     * @param radius the radius of each individual circle.
     * @return The generated Node list.
     */
    static QList<Node*> generateHexArray(int size, qreal radius);


    int getId();
    void setId(int value);

    qreal getRadius();
    void setRadius(const qreal value);

    QColor getColor();
    void setColor(const QColor &value);

    //neibhours must maintain reflexivity.
    void addNeibhour(Node* node);
    void delNeibhour(Node* node);
    bool isNeibhour(Node* node);
    void sortNeibhours();
    QList<Node*> getNeibhours();
    int getNeibhourCount();

    void setPosition(QPointF &position);
    QPointF getPosition();
    bool hasPosition();
    void delPosition();

    bool hasFullFlower();



protected:
    int id;
    QPointF position;
    qreal radius;
    QList<Node*> neibhours;
    QColor color;
    bool bHasPosition=false;
    bool sortedNeibhours=false;
};

#endif // NODE_HPP
