#ifndef EDGE_HPP
#define EDGE_HPP

namespace Circles{
    namespace Graph{

    /**
     * Defines an edge between two nodes in a graph. An edge is defined by the two nodes that it is co-incident to.
     * The two defining nodes are constrained so that the lower-order one is the first argument.
     */
    class Edge
    {
    public:
        /**
         * Default constructor. Initializes the edge to (-1, -1)
         */
        Edge();

        /**
         * Construct an edge. The first node should be lower-order than the second.
         * @param x index of the first node of the edge
         * @param y index of the second node of the edge.
         */
        Edge(int x, int y);

        Edge(const Edge& other);

        Edge& operator=(const Edge& other);

        friend bool operator==(const Edge& lhs, const Edge& rhs);

        int getX();
        int getY();

        void setX(int x);
        void setY(int y);

        void set(int x, int y);

    private:
        int x;
        int y;
    };

    bool operator==(const Edge& lhs, const Edge& rhs);

    }
}

#endif // EDGE_HPP
