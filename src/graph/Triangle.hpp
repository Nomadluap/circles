#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "graph/Node.hpp"

using namespace Circles;

namespace Circles{
    namespace Graph{


    /**
     * Represents a triangle on a graph G.
     */
    class Triangle{
    public:
        Triangle();
        Triangle(Node t1, Node t2, Node t3);
        Triangle(const Triangle& other);
        Triangle& operator=(const Triangle& other);

        bool operator==(const Triangle& rhs) const;

        bool operator<(const Triangle& rhs) const;

        Node p1() const;
        Node p2() const;
        Node p3() const;

    private:
        Circles::Graph::Node p1_;
        Circles::Graph::Node p2_;
        Circles::Graph::Node p3_;


    };

    } // namespace packing
} // namespace Circles


#endif // TRIANGLE_HPP
