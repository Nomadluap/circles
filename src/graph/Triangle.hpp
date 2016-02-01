#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <graph/Graph.hpp>

using Circles::Graph::Node;

namespace Circles{
    namespace Graph{

    /**
     * Represents a triangle on a graph G.
     */
    struct Triangle{
        Node p1;
        Node p2;
        Node p3;

        bool operator==(const Triangle& rhs){
            return (this->p1 == rhs.p1) && (this->p2 == rhs.p2) && (this->p3 == rhs.p3);
        }
    };

    } // namespace packing
} // namespace Circles


#endif // TRIANGLE_HPP
