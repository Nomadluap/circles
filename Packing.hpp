#ifndef PACKING_HPP
#define PACKING_HPP

//to prevent cyclic imports.
class Packing;

#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include "Node.hpp"
#include "Circle.hpp"

/**
 * @brief The Packing class is an abstract class which defines a circle packing.
 * A circle packing may be either euclidean or hyperbolic, as found in
 * EuclideanPacking and HyperbolicPacking.
 */
class Packing : public QGraphicsScene
{
    Q_OBJECT
public:
    Packing();

    bool getDrawCenters();
    bool getDrawLinks();
    bool getDrawCircles();
    bool getDrawIndicies();

    void addNode(Node *n);


public slots:
    void setDrawCenters(bool d);
    void setDrawLinks(bool d);
    void setDrawCircles(bool d);
    void setDrawIndicies(bool d);



protected:
    virtual void addCircle(Node *n);

    bool drawCenters=true;
    bool drawLinks=true;
    bool drawCircles=true;
    bool drawIndicies=true;

    QList<Node*> nodes;
    QList<Node*> boundaryNodes;
    QList<Circle*> circles;



};

#endif // PACKING_HPP
