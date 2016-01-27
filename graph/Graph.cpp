#include "Graph.hpp"
using namespace Circles::Graph;

Circles::Graph::Graph::Graph(): // default constructor
    edges(new QHash<Node, QSet<Node> >()),
    boundaryNodes(new QSet<Node>())
{
    //nothing here.

}

Circles::Graph::Graph::Graph(const Graph& other): // copy constructor
    edges(new QHash<Node, QSet<Node> >(*other.edges)),
    boundaryNodes(new QSet<Node>(*other.boundaryNodes))
{
    //nothing here either boss.
}

Circles::Graph::Graph::Graph(Graph&& other): // move constructor
    edges(std::move(other.edges)),
    boundaryNodes(std::move(other.boundaryNodes))
{
    //nothing to do here.
}

Graph& Circles::Graph::Graph::operator=(const Graph& other) // copy assignment
{
    edges = std::make_unique<QHash<Node, QSet<Node> > >(*other.edges);
    boundaryNodes = std::make_unique<QSet<Node> >(*other.boundaryNodes);
    return *this;
}

Graph& Circles::Graph::Graph::operator=(Graph&& other) // move assignment
{
    edges = std::move(other.edges);
    boundaryNodes = std::move(other.boundaryNodes);
    return *this;
}


void Circles::Graph::Graph::addEdge(Node x, Node y)
{
    if(x == y) return; // no self-edges allowed.

    // we need to add y to the edge set of x, and x to the edge set of y to maintain symmetry
    if(!this->edges->contains(x)) this->edges->insert(x, QSet<Node>());
    if(!this->edges->contains(y)) this->edges->insert(y, QSet<Node>());

    auto xset = this->edges->take(x);
    xset.insert(y);
    this->edges->insert(x, std::move(xset));

    auto yset = this->edges->take(y);
    yset.insert(x);
    this->edges->insert(y, std::move(yset));

}

void Graph::addEdge(Edge e)
{
    this->addEdge(e.getX(), e.getY());
}

void Circles::Graph::Graph::removeEdge(Node x, Node y)
{
    if(!this->hasEdge(x, y)) return;

    // the edge exists and must be terminated. (Read in arnold Schwarzenegger voice)
    auto xset = this->edges->take(x);
    xset.remove(y);
    this->edges->insert(x, std::move(xset));

    auto yset = this->edges->take(y);
    yset.remove(x);
    this->edges->insert(y, std::move(yset));

}

bool Circles::Graph::Graph::hasEdge(Node x, Node y)
{
    if(!this->edges->contains(x) || !this->edges->contains(y)) return false; // edge does not exist since nodes do not exist.
    else if(!this->edges->value(x).contains(y) || !this->edges->value(y).contains(x)) return false; // edge not defined.
    else return true; //otherwise teh edge exists.
}

std::unique_ptr<QList<Node> > Circles::Graph::Graph::getNodes()
{
    auto nodes = std::make_unique<QList<Node> >(this->edges->keys());
    return nodes;
}

std::unique_ptr<QList<Edge> > Circles::Graph::Graph::getEdges()
{
    // iterate over each node. Take all nodes with order greater. Form Edge.
    QList<Edge>* edgelist = new QList<Edge>();
    for(Node x: this->edges->keys()){
        for(Node y: this->edges->value(x)){
            if( x < y) edgelist->append(Edge(x, y));
        }
    }
    return std::unique_ptr<QList<Edge> >(edgelist);
}

