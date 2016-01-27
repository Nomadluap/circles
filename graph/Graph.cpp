#include "Graph.hpp"
using namespace Circles::Graph;

Circles::Graph::Graph::Graph(): // default constructor
    edges(new QHash<Node, QSet<Node> >()),
    boundaryNodes(new QSet<Node>()),
    is_edges_sorted(new QHash<Node, bool>),
    is_boundary_sorted(false),
    is_boundary_valid(false)
{
    //nothing here.

}

Circles::Graph::Graph::Graph(const Graph& other): // copy constructor
    edges(new QHash<Node, QSet<Node> >(*(other.edges))),
    boundaryNodes(new QSet<Node>(*(other.boundaryNodes))),
    is_edges_sorted(new QHash<Node, bool>(*(other.is_edges_sorted))),
    is_boundary_sorted(*other.is_boundary_sorted),
    is_boundary_valid(*other.is_boundary_valid)
{
    //nothing here either boss.
}

Circles::Graph::Graph::Graph(Graph&& other): // move constructor
    edges(std::move(other.edges)),
    boundaryNodes(std::move(other.boundaryNodes)),
    is_edges_sorted(std::move(other.is_edges_sorted)),
    is_boundary_sorted(other.is_boundary_sorted),
    is_boundary_valid(other.is_boundary_valid)
{
    //nothing to do here.
}

Graph& Circles::Graph::Graph::operator=(const Graph& other) // copy assignment
{
    this->edges = std::make_unique<QHash<Node, QSet<Node> > >(*(other.edges));
    this->boundaryNodes = std::make_unique<QSet<Node> >(*(other.boundaryNodes));
    this->is_edges_sorted = std::make_unique<Hash<Node, bool>(*(other.is_edges_sorted));
    this->is_boundary_sorted = other.is_boundary_sorted;
    this->is_boundary_valid = other.is_boundary_valid;
    return *this;
}

Graph& Circles::Graph::Graph::operator=(Graph&& other) // move assignment
{
    this->edges = std::move(other.edges);
    this->boundaryNodes = std::move(other.boundaryNodes);
    this->is_edges_sorted = std::move(other.is_edges_sorted);
    this->is_boundary_sorted = other.is_boundary_sorted;
    this->is_boundary_valid = other.is_boundary_valid;
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

    // invalidates sorting on both nodes as well as the boundary.
    this->is_edges_sorted->insert(x, false);
    this->is_edges_sorted->insert(y, false);
    this->is_boundary_valid = false;
    this->is_boundary_sorted = false;

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

    // invalidates sorting on both nodes as well as the boundary.
    this->is_edges_sorted->insert(x, false);
    this->is_edges_sorted->insert(y, false);
    this->is_boundary_valid = false;
    this->is_boundary_sorted = false;

}

bool Circles::Graph::Graph::hasEdge(Node x, Node y) const
{
    if(!this->edges->contains(x) || !this->edges->contains(y)) return false; // edge does not exist since nodes do not exist.
    else if(!this->edges->value(x).contains(y) || !this->edges->value(y).contains(x)) return false; // edge not defined.
    else return true; //otherwise teh edge exists.
}

bool Graph::hasFullFlower(Node n)
{
    //requires sorted neighbours in the nodes' edge list
    auto sn = this->sortedNeighbours(n);

    if(sn.first() == sn.last() && this->is_edges_sorted->value(n)) return true;
    else return false;

}

bool Graph::isBoundary(Node n)
{
    return !this->hasFullFlower(n);
}

std::unique_ptr<QList<Node> > Circles::Graph::Graph::getNodes() const
{
    auto nodes = std::make_unique<QList<Node> >(this->edges->keys());
    return nodes;
}

std::unique_ptr<QList<Edge> > Circles::Graph::Graph::getEdges() const
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

QList<Node>& Circles::Graph::Graph::neighbours(Node i) const
{
    return this->edges->value(i);
}

QList<Node> Graph::sortedNeighbours(Node n)
{
    if(!this->is_edges_sorted->value(n)) this->sortNeighbours(i);
    return QList<Node>(this->edges->value(n));

}

QList<Node> Graph::boundary()
{
    if(!this->is_boundary_sorted) this->sortBoundary();
    return QList<Node>(*(this->boundaryNodes));

}

void Graph::computeBoundary()
{
    // Each node that does not have a full flower is a boundary node.
    for(Node n: *(this->edges)){
        if(!hasFullFlower(n)) boundaryNodes->append(n);
    }
    this->is_boundary_valid = true;
}

void Graph::sortBoundary()
{
    // there is probably a better way to do this.
    if(!this->is_boundary_valid) this->computeBoundary();
    // pick one boundary node. Iterate through nodes, finding ones that either can go on the left or right of the
    // existing list. Keep going until we either process the whole original list or we go through the entire list
    // without finding anything
    auto newlist = std::make_unique<QList<Node>>(new QList<Node>());
    newlist->append(this->boundaryNodes->takeFirst());
    while(this->boundaryNodes->length() != 0){ // we exit early if we get a partition of the set
        bool fullCycle = true;
        for(Node n: *(this->boundaryNodes)){
            if(this->edges->value(newlist->first()).contains(n)){
                this->boundaryNodes->removeFirst(n);
                newlist->prepend(n);
                fullCycle = false;
            }
            else if(this->edges->value(newlist->last()).contains(n)){
                this->boundaryNodes->removeFirst(n);
                newlist->append(n);
                fullCycle = false;
            }
        }
        if(fullCycle) return; //boundary set does not form a single ring.
    }
    //we've sorted the whole boundary and can calim so
    this->boundaryNodes = newlist;
    this->is_boundary_sorted = true;

}

void Graph::sortNeighbours(Node n)
{
    QList<Node> oldlist(this->edges->value(n));
    QList<Node> newlist();
    newlist.append(oldlist.takeFirst());
    while(oldlist.length() != 0){
        bool fullCycle = true;
        for(Node m: oldlist){
            if(this->edges->value(newlist->first()).contains(m)){
                oldlist.removeFirst(m);
                newlist->prepend(m);
                fullCycle = false;
            }
            else if(this->edges->value(newlist->last()).contains(m)){
                oldlist.removeFirst(m);
                newlist->append(m);
                fullCycle = false;
            }
        }
        if(fullCycle) return; //boundary set does not form a single ring.
    }
    this->edges->insert(n, std::move(newlist));
    this->is_edges_sorted->value(n) = true;
}

