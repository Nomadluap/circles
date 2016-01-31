#ifndef EUCLIDPACKING_HPP
#define EUCLIDPACKING_HPP

#include <QHash>
#include <memory>
#include "graph/Graph.hpp"
#include "packing/Packing.hpp"
namespace Circles{
    namespace Packing{

    /**
     * A packing on the euclidean plane. Holds EuclidCircles.
     */
    class EuclidPacking : public Packing
    {
    public:
        /**
         * Construct an empty packing with no underlying graph. (don't use this)
         * Underlying graph will be empty.
         */
        EuclidPacking();

        /**
         * Construct a new euclidean packing with a specified underlying graph.
         * All circles will be initialized to radius 1 and center 0,0.
         * @param g pointer to underlying graph.
         */
        EuclidPacking(std::shared_ptr<Graph::Graph> g);

        /**
         * Copy reference to node as well as deep-copy of circles.
         */
        EuclidPacking(const EuclidPacking& other);

        EuclidPacking(EuclidPacking&& other);

        EuclidPacking& operator=(const EuclidPacking& other);

        EuclidPacking& operator=(EuclidPacking&& other); //move assignment.

        //EuclideanPackings are considered to be equal if they share a graph and have Circles with identical parameters.
        friend bool operator==(const EuclidPacking& lhs, const EuclidPacking& rhs);

        virtual void layout(int centerCircle) override;

        virtual qreal angle(const QPointF &p, const QPointF &p1, const QPointF &p2) const override;

    protected:
        virtual void spawnCircles() override;
    };

    bool operator==(const EuclidPacking& lhs, const EuclidPacking& rhs);

    }
}

#endif // EUCLIDPACKING_HPP
