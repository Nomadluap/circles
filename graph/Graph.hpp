#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QHash>
#include <QSet>
#include <memory>
#include <graph/Edge.hpp>

namespace Circles{

    namespace Graph{

    //typedefs to simplify the language.
    typedef int Node;

    /**
     * Implements a mathematical graph.
     * The graph also stores information about the boundary nodes and edges.
     */
    class Graph
    {
    public:
        Graph();
        Graph(const Graph& other); //copy constructor
        Graph(Graph&& other); //move constructor
        Graph& operator=(const Graph& other); //assignment operator
        Graph& operator=(Graph&& other); //move assignment

        /**
         * Add an edge to the graph. Note that edges are symmetric. eg. AB == BA
         * @param x index of one end of the edge
         * @param y indexc of the other end of the edge.
         */
        void addEdge(Node x, Node y);
        void addEdge(Edge e);

        /**
         * Removes an edge from the graph, if it exists.
         * @param x index of first node which defines edge
         * @param y index of second node which defines edge
         */
        void removeEdge(Node x, Node y);

        /**
         * Deterimine if the graph contains the edge between two nodes.
         * @param x the first node
         * @param y the second node
         * @return True if the edge is contained in the graph, otherwise false.
         */
        bool hasEdge(Node x, Node y);

        /**
         * Get the set of nodes that are present in the graph.
         * @return Set of the nodes present in the graph
         */
        std::unique_ptr<QList<Node> > getNodes();

        /**
         *  Get a set of all edges in the graph.
         * The pairs returned will always have the lowest-order node index first.
         * @return A set of node pairs representing the edges of the graph.
         */
        std::unique_ptr<QList<Edge> > getEdges();

    private:
        std::unique_ptr<QHash<Node, QSet<Node> > > edges;
        std::unique_ptr<QSet<Node> > boundaryNodes;
    };

    }
}

#endif // GRAPH_HPP
