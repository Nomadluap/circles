#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QHash>
#include <QList>
#include <memory>
#include "graph/Edge.hpp"
#include "graph/Triangle.hpp"
#include "graph/Node.hpp"

namespace Circles{

    namespace Graph{

    /**
     * Implements a mathematical graph.
     * The graph also stores information about the boundary nodes and edges.
     */
    class Graph
    {
    public:
        /**
         * Generate a graph of a hexagonal tiling with the specified width and height
         * @param w the width
         * @param h the height
         */
        static std::shared_ptr<Circles::Graph::Graph> generateHexArray(int w, int h);

        Graph();
        Graph(const Graph& other); //copy constructor
        Graph(Graph&& other); //move constructor
        Graph& operator=(const Graph& other); //assignment operator
        Graph& operator=(Graph&& other); //move assignment

        // two graphs are equal if they share the same edge set.
        friend bool operator==(const Graph& lhs, const Graph& rhs);


        /**
         * Add an edge to the graph. Note that edges are symmetric. eg. AB == BA
         * @param x index of one end of the edge
         * @param y indexc of the other end of the edge.
         */
        void addEdge(Node x, Node y);
        void addEdge(Edge e);

        /**
         * Removes an edge from the graph, if it exists.
         * If a node has no neighbours, then it is removed from the map.
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
        bool hasEdge(Node x, Node y) const ;

        /**
         * Determine if a specified node has a full flower, ie. if the neighbours of the node form a ring around it.
         * @param n node to query
         * @return True if the node has a full flower, otherwise False.
         */
        bool hasFullFlower(Node n) ;

        /**
         * Deterimine if a node is in the boundary of the graph.
         * @param n The node to query
         * @return True if the node is in the boundary, otherwise false.
         */
        bool isBoundary(Node n);

        /**
         * Get the set of nodes that are present in the graph.
         * @return Set of the nodes present in the graph
         */
        QList<Node> getNodes() const;

        /**
         * Determine if a graph has a specified node.
         * @param n node to query
         * @return True if the node exists, false otherwise.
         */
        bool hasNode(Node n) const;

        /**
         * Removes a node from the graph, also removing all edges that reference it.
         * @param n The ndoe to remove
         */
        void removeNode(Node n);

        /**
         *  Get a set of all edges in the graph.
         * The pairs returned will always have the lowest-order node index first.
         * @return A set of node pairs representing the edges of the graph.
         */
        QList<Edge> getEdges() const;

        /**
         * Get the set of nodes which are adjacent to the specified node. Does not require sorting of
         * the nodes lst.
         * @param i Node to query
         * @return  Set of nodes which are ajacent to the specified node.
         */
        QList<Node> neighbours(Node i) const;

        /**
         * Attempt to get a list of the ajacent nodes of a specified node such that two nodes ajacent in the list
         * are also ajacent in the graph.
         * @param i
         * @return The sorted list, if it exists. Otherwise return an empty list.
         */
        QList<Node> sortedNeighbours(Node n);

        /**
         * Get a sorted list of nodes that make up the boundary of the graph.
         * @return sorted list of nodes.
         */
        QList<Node> boundary();

        /**
         * Get the list of triangles in the graph. Each triangle will be included exactly once.
         * @return List of triangles. First point will have lowest order, and the last point will have the highest.
         */
        QList<Triangle> triangles() const;


    private:
        /**
         * Compute the boundary nodes in the graph.
         */
        void computeBoundary();

        /**
         * Sort the boundary so that ajacent nodes are next to eachother in the boundary list.
         */
        void sortBoundary();

        /**
         * Sort the neighbours of a node so that ajacent nodes in the list are neighbours.
         * @param n Node whose neighbours to sort.
         */
        void sortNeighbours(Node n);

        std::unique_ptr<QHash<Node, QList<Node> > > edges;
        std::unique_ptr<QList<Node> > boundaryNodes;
        std::unique_ptr<QHash<Node, bool> > is_edges_sorted;
        bool is_boundary_sorted;
        bool is_boundary_valid;
    };

    bool operator==(const Graph& lhs, const Graph& rhs);
    bool operator!=(const Graph& lhs, const Graph& rhs);

    } // namespace Graph
} // namespace Circles

#endif // GRAPH_HPP
