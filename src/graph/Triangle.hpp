#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "graph/Node.hpp"

using namespace Circles;

namespace Circles{
    namespace Graph{


    /**
     * Represents a triangle on a graph G.
     */
    struct Triangle{
        Circles::Graph::Node p1;
        Circles::Graph::Node p2;
        Circles::Graph::Node p3;

        bool operator==(const Triangle& rhs) const{
            return (this->p1 == rhs.p1) && (this->p2 == rhs.p2) && (this->p3 == rhs.p3);
        }

        bool operator<(const Triangle& rhs) const{
            if(this->p1 < rhs.p1) return true;
            else if(this->p2 < rhs.p2) return true;
            else if(this->p3 < rhs.p3) return true;
            else return false;
        }
    };

    } // namespace packing
} // namespace Circles


#endif // TRIANGLE_HPP
