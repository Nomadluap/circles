#ifndef PACKING_HPP
#define PACKING_HPP

#include <memory>
#include <QList>
#include <QMap>
#include <graph/Graph.hpp>
#include <packing/Circle.hpp>
using namespace Circles;

namespace Circles{
    namespace Packing{

    /**
     * Abstract base class of the EuclidPacking and the HyperPacking, which represent a circle packing over a specific
     * Graph.
     *
     * Circle packings consist of a number of circles which lie tangent to eachother.
     */
    class Packing
    {
    public:
        /**
         * Set boundary circles to be of a specified radius and then modify other circles to form a proper packing.
         * @param epsilon tolerance value when determining angle sum
         * @param outerRadius Radius of the boundary circles.
         */
        void repack(qreal epsilon, qreal outerRadius);

        /**
         * Lay out the circles such that neighbourstd:ng circles are tangent to one another.
         * @param centerNode index of the circle to place at the center of the packing.
         */
        virtual void layout(int centerCircle) = 0;

        /**
         * Compute the angle <p,p1,p2 in the local space of the packing.
         * @param point which represents the vertex point.
         * @param point which defines one ray of the angle.
         * @param point which defines the other ray of the angle.
         * @return The angle formed by the points, in radians in range [0, pi]
         */
        virtual qreal angle(const QPointF& p, const QPointF& p1, const QPointF& p2) const = 0;

        /**
         * Get a reference to the underlying graph of the packing.
         * @return const reference to graph object underlying the packing.
         */
        const Graph::Graph& graph() const;

        /**
         * Get a list of all circles in the packing, addressable by their indicies.
         * Note that these circles are not referenced to the circles that still exist in the packing.
         * @return Qhash of index -> circle elements for the packing.
         */
        QMap<int, std::shared_ptr<Circle>> circles() const;

        /**
         * Get a circle based on its index.
         * @param index index of the circle
         * @return const reference to circle with specified index.
         */
        const Circle& circle(int index) const;

        /**
         * Determine if a circle has a full flower
         * @param c circle to query
         * @return True if the circle has a full flower, false otherwise.
         */
        bool hasFullFlower(const Circle& c) const;

        /**
         * Get the list of circles which are neighbours to the circle c.
         * @param c The circle to query
         * @return  List of pointers to the ajacent circles.
         */
        QList<std::shared_ptr<Circle>> neighbours(const Circle& c);


    protected:

        /**
         * Compute the sum of the angles formed by a circle and its neighbours.
         * @param c the circle to consider.
         * @return sum of angles in radians, in range [0, 2pi]
         */
        qreal anglesum(const Circle& c) const;

        /**
         * Update the circle list so that every graph node has exactly one circle assosciated with it.
         */
        virtual void spawnCircles() = 0;


        std::shared_ptr<Graph::Graph> _graph;
        // since Circles are ordered, we can sort this list for easy lookups.
        QMap<int, std::shared_ptr<Circle> > _circles;
    };
    }
}

#endif // PACKING_HPP

//TODO: HyperPacking
//TODO: :Connect packings to display.
