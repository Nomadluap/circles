#ifndef EUCLIDPACKING_HPP
#define EUCLIDPACKING_HPP

#include <QHash>
#include <memory>
#include <QRectF>
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
         * Construct a EuclideanPacking of a hexagonal array, using circles of a specified radius.
         * @param w number of circles in the horizontal direction
         * @param h number of circles in the vertical direction.
         * @param radius radius of the circles
         * @return shared pointer to the packing generated.
         */
        static std::shared_ptr<EuclidPacking> generateHexArray(int w, int h, QPointF topleft, qreal radius);

        /**
         * Construct a euclideanPacking of a hexagonal tiling of the specified area of circles with a specified radius
         * @param area The area to cover
         * @param radius The radius of the circles
         * @return shared pointer to the packing generated.
         */
        static std::shared_ptr<EuclidPacking> generateHexArray(QRectF area, qreal radius);

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

        virtual void layout() override;

        virtual qreal angle(qreal r, qreal ra, qreal rb) const override;


    protected:
        virtual void spawnCircles() override;
    };

    bool operator==(const EuclidPacking& lhs, const EuclidPacking& rhs);

    }
}

#endif // EUCLIDPACKING_HPP
