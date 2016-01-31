#ifndef HYPERPACKING_HPP
#define HYPERPACKING_HPP

#include <QHash>
#include <memory>
#include "graph/Graph.hpp"
#include "packing/Packing.hpp"
namespace Circles{
    namespace Packing{

    /**
     * A packing on the euclidean plane. Holds EuclidCircles.
     */
    class HyperPacking : public Packing
    {
    public:
        /**
         * Construct an empty packing with no underlying graph. (don't use this)
         * Underlying graph will be empty.
         */
        HyperPacking();

        /**
         * Construct a new euclidean packing with a specified underlying graph.
         * All circles will be initialized to radius 1 and center 0,0.
         * @param g pointer to underlying graph.
         */
        HyperPacking(std::shared_ptr<Graph::Graph> g);

        /**
         * Copy reference to node as well as deep-copy of circles.
         */
        HyperPacking(const HyperPacking& other);

        HyperPacking(HyperPacking&& other);

        HyperPacking& operator=(const HyperPacking& other);

        HyperPacking& operator=(HyperPacking&& other); //move assignment.

        //HyperPackings are considered to be equal if they share a graph and have Circles with identical parameters.
        friend bool operator==(const HyperPacking& lhs, const HyperPacking& rhs);

        virtual void layout(int centerCircle) override;

        virtual qreal angle(const QPointF &p, const QPointF &p1, const QPointF &p2) const override;

    protected:
        virtual void spawnCircles() override;
    };

    bool operator==(const HyperPacking& lhs, const HyperPacking& rhs);

    }
}

#endif // HYPERPACKING_HPP
