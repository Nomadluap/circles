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
     * Generates a hex tiling of circles with specified radius such that circles
     * are guaranteed to cover at least the specified area
     * @param area the area to cover
     * @param radius radius of the circles
     * @return list of nodes representing the circles created.
     */
    static QList<Node*> generateHexArray(const QRectF &area, qreal radius);

    /**
     *
     * @param startpos  the location of the top-left circle
     * @param w width, in circles, of the circle array
     * @param h height, in circles, of the circle array. Should be odd.
     * @param radius the radius of each circle.
     * @return list of nodes representing the circles created.
     */
    static QList<Node*> generateHexArray(const QPointF &startpos, int w, int h, qreal radius);


    int getId();
    void setId(int value);

    qreal getRadius();
    void setRadius(const qreal value);

    QColor getColor();
    void setColor(const QColor &value);

    //neibhours must maintain reflexivity.
    void addNeibhour(Node* node);
    void delNeibhour(Node* node);
    void purgeNeibhours();
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
