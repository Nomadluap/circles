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
         */
        EuclidPacking();

        /**
         * Construct a new euclidean packing with a specified underlying graph.
         * All circles will be initialized to radius 1 and center 0,0.
         * @param g pointer to underlying graph.
         */
        EuclidPacking(std::shared_ptr<Graph::Graph> g);

        /**
         * Construct a new euclidean packing with a specified underlying graph and pre-defined circles.
         * @param g the underlying graph.
         * @param circles List of circles to pre-setup. Dupliecate circles and circles which do not correspond to
         *  nodes present in the graph will be removed. Any nodes without circles in the list will have their circles
         *  initialized to radius 1.0 and center 0,0.
         */
        EuclidPacking(std::shared_ptr<Graph::Graph> g,  const QList<Circle*>& circles);

        EuclidPacking(const EuclidPacking& other);

        EuclidPacking(EuclidPacking&& other);

        EuclidPacking& operator=(const EuclidPacking& other);

        EuclidPacking& operator=(EuclidPacking&& other); //move assignment.

        //EuclideanPackings are considered to be equal if they share a graph and have Circles with identical parameters.
        friend bool operator==(const EuclidPacking& lhs, const EuclidPacking& rhs);
    };

    bool operator==(const EuclidPacking& lhs, const EuclidPacking& rhs);

    }
}

#endif // EUCLIDPACKING_HPP
