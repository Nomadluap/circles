#ifndef PACKINGCOORDINATE_HPP
#define PACKINGCOORDINATE_HPP

#include <graph/Graph.hpp>
#include <graph/Triangle.hpp>

using Circles::Graph::Node;
using Circles::Graph::Triangle;

namespace Circles{
    namespace Packing{

    /**
      * Represents a coordinate on a circle packing using a Barycenter-like coordinate system.
      *
      * Given three non-colinear points p1, p2, p3, we may locate any point inside the triangle
      * formed by the points in the form a*p1 + b*p2 + c*p3, where a+b+c == 0. This gives us two
      * degrees of freedom in our point. Defining our points in this way allows us to carry them
      * in a continuous way when the space is mapped conformally into different shapes, independent
      * of the geometry of the space.
      *
      * It is the responsibility of the caller to ensure that restrictions on the coefficients
      * are met, as this class has no knowledge of the actual geometry of the packing.
      *
      * This is an immutable class.
      */
    class PackingCoordinate
    {
    public:
        /**
         * Construct a default PackingCoordinate with a 0-triangle and all co-efficients set to zero
         */
        PackingCoordinate();

        /**
         * Construct a PackingCoordinate with respect to the specified triangle.
         *
         * Remember that it is imperative to remember that a+b+c == 1;
         * @param t Triangle with which to base measurement
         * @param a co-efficient of the first term
         * @param b co-efficient of the second term
         * @param c co-efficient of the third term.
         */
        PackingCoordinate(Triangle t, qreal a, qreal b, qreal c);

        PackingCoordinate(const PackingCoordinate& other);

        PackingCoordinate& operator=(const PackingCoordinate& other);

        bool operator==(const PackingCoordinate& other);

        qreal a() const;
        qreal b() const;
        qreal c() const;

        const Triangle& triangle() const;

    private:
        // Indices of nodes
        Triangle _t;

        // Co-efficients.
        qreal _a;
        qreal _b;
        qreal _c;
    };

    } // namespace packing
} // namespace Circles


#endif // PACKINGCOORDINATE_HPP
