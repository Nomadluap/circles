#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QHash>
#include <QSet>

/**
 * Implements a mathematical graph.
 */
class Graph
{
public:
    Graph();
    Graph(const Graph& other); //copy constructor
    Graph& operator=(const Graph& other); //assignment operator

    void addEdge(int x, int y);
    void removeEdge(int x, int y);
    bool hasEdge(int x, int y);

private:
    QHash<int, QSet<int> > edges;
    QSet<int> boundaryNodes;
};

#endif // GRAPH_HPP
