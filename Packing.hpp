#ifndef PACKING_HPP
#define PACKING_HPP

//to prevent cyclic imports.
class Packing;



#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include "Node.hpp"
#include "Circle.hpp"
#include "Connector.hpp"
#include "PackingType.hpp"

/**
 * @brief The Packing class is an abstract class which defines a circle packing.
 * A circle packing may be either euclidean or hyperbolic, as found in
 * EuclideanPacking and HyperbolicPacking.
 */
class Packing : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief Generates a hex-tile packing that is suitable for intersection
     * with arbitrary shapes to begin a hyperbolic repacking
     * @param size the number of circles across to generate
     * @param radius the radius of each individual circle.
     * @return The generated packing.
     */
    static Packing *generateHexPacking(int size, qreal radius);

    Packing(PackingType type = PackingType::EuclideanPacking);

    /**
     * @brief setPackingType sets the geometry of the packing
     * @param type either PackingType::EuclideanPacking or
     * PackingType::HyperbolicPacking
     */
    void setPackingType(PackingType type);

    /**
     * @brief getType
     * @return geometry of the packing.
     */
    PackingType getType();

    bool getDrawCenters();
    bool getDrawLinks();
    bool getDrawCircles();
    bool getDrawIndicies();

    /**
     * @brief addNode Adds a node to the packing.
     * @param n node to add.
     */
    void addNode(Node *n);

    /**
     * @brief Add a node without re-computing connectors. You must manually call
     * recompute_connectors() after adding all nodes.
     * @param n node to add
     */
    void addNode_fast(Node *n);


    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) Q_DECL_OVERRIDE;

    /**
     * @brief recomputeConnectors re-computes the coordinates of the connectors
     * between circle-centers.
     */
    void recomputeConnectors();

public slots:
    void setDrawCenters(bool d);
    void setDrawLinks(bool d);
    void setDrawCircles(bool d);
    void setDrawIndicies(bool d);

    /**
     * @brief repack Compute the radii of the circles that will result in
     * an actual packing.
     * @param epsilon epsilon value to determine completeness. Small.
     * @param outerRadius Radius of boundary circles. Large.
     */
    void repack(qreal epsilon, qreal outerRadius);

    /**
     * @brief lay-out circles once radii have been computed.
     * @param centerCircle index of circle to place at center of plane/disc
     */
    void layout(int centerCircle);



private:
    enum class SelectionState{NoneSelected, PartialSelection, f};
    /**
     * @brief angle Compute the angle formed by the tangent circles of 3 nodes.
     * @param r the center node of the angle
     * @param ra one leg of the angle
     * @param rb one leg of the angle
     * @return the angle formed, in radians.
     */
    qreal angle(Node* r, Node* ra, Node* rb);
    qreal angle_euclidean(Node* r, Node* ra, Node* rb);
    qreal angle_hyperbolic(Node* r, Node* ra, Node* rb);

    /**
     * @brief anglesum returns sum of all angles formed with ajacent nodes
     * @param r node
     * @return angle sum
     */
    qreal anglesum(Node* r);

    void addCircle(Node *n);



    /**
     * @brief isInterior Determine if given node is interior to packing.
     * @param n node
     * @return true if node is interior to packing. Otherwise False.
     */
    bool isInterior(Node* n);


    void drawForeground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    bool drawCenters=true;
    bool drawLinks=true;
    bool drawCircles=true;
    bool drawIndicies=true;

    PackingType type;

    QList<Node*> nodes;
    QList<Node*> boundaryNodes;
    QList<Circle*> circles;
    QList<Connector*> connectors;



};

#endif // PACKING_HPP
