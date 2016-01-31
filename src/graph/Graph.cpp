#include "Graph.hpp"
using namespace Circles::Graph;

Circles::Graph::Graph::Graph(): // default constructor
    edges(new QHash<Node, QList<Node> >()),
    boundaryNodes(new QList<Node>()),
    is_edges_sorted(new QHash<Node, bool>()),
    is_boundary_sorted(false),
    is_boundary_valid(false)
{
    //nothing here.

}

Circles::Graph::Graph::Graph(const Graph& other): // copy constructor
    edges(new QHash<Node, QList<Node> >(*(other.edges))),
    boundaryNodes(new QList<Node>(*(other.boundaryNodes))),
    is_edges_sorted(new QHash<Node, bool>(*(other.is_edges_sorted))),
    is_boundary_sorted(other.is_boundary_sorted),
    is_boundary_valid(other.is_boundary_valid)
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
    // this->edges = std::m<QHash<Node, QList<Node> >(*(other.edges));
    this->edges = std::make_unique<QHash<Node, QList<Node> > >(*(other.edges));
    this->boundaryNodes = std::make_unique<QList<Node> >(*(other.boundaryNodes));
    this->is_edges_sorted = std::make_unique<QHash<Node, bool> >(*(other.is_edges_sorted));
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
    if(!this->edges->contains(x)) this->edges->insert(x, QList<Node>());
    if(!this->edges->contains(y)) this->edges->insert(y, QList<Node>());

    auto xset = this->edges->take(x);
    xset.append(y);
    this->edges->insert(x, std::move(xset));

    auto yset = this->edges->take(y);
    yset.append(x);
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
    QList<Node> xset = this->edges->take(x);
    xset.removeAll(y);
    this->edges->insert(x, std::move(xset));

    QList<Node> yset = this->edges->take(y);
    yset.removeAll(x);
    this->edges->insert(y, std::move(yset));

    // Remove any node with no neighbours.
    if(edges->value(x).length() == 0) edges->remove(x);
    if(edges->value(y).length() == 0) edges->remove(y);

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
    for(int i = 0; i < sn.length() - 1; ++i){
        if(!this->hasEdge(sn.at(i), sn.at(i+1))) return false;
    }
    if(this->hasEdge(sn.first(), sn.last()) && this->is_edges_sorted->value(n)) return true;
    else return false;

}

bool Graph::isBoundary(Node n)
{
    return !this->hasFullFlower(n);
}

QList<Node> Circles::Graph::Graph::getNodes() const
{
    auto nodes = QList<Node>(this->edges->keys());
    return nodes;
}

bool Circles::Graph::Graph::hasNode(Node n) const
{
    return this->edges->contains(n);
}

QList<Edge> Circles::Graph::Graph::getEdges() const
{
    // iterate over each node. Take all nodes with order greater. Form Edge.
    QList<Edge> edgelist;
    for(Node x: this->edges->keys()){
        for(Node y: this->edges->value(x)){
            if( x < y) edgelist.append(Edge(x, y));
        }
    }
    return QList<Edge>(std::move(edgelist));
}

QList<Node> Circles::Graph::Graph::neighbours(Node i) const
{
    return this->edges->value(i);
}

QList<Node> Circles::Graph::Graph::sortedNeighbours(Node n)
{
    if(!this->is_edges_sorted->value(n)){
        this->sortNeighbours(n);
    }
    return this->edges->value(n);

}

QList<Node> Circles::Graph::Graph::boundary()
{
    if(!this->is_boundary_sorted) this->sortBoundary();
    return QList<Node>(*(this->boundaryNodes));

}

void Circles::Graph::Graph::computeBoundary()
{
    // Each node that does not have a full flower is a boundary node.
    for(Node n: this->edges->keys()){
        if(!hasFullFlower(n)) boundaryNodes->append(n);
    }
    this->is_boundary_valid = true;
}

void Circles::Graph::Graph::sortBoundary()
{
    // there is probably a better way to do this.
    if(!this->is_boundary_valid) this->computeBoundary();
    // pick one boundary node. Iterate through nodes, finding ones that either can go on the left or right of the
    // existing list. Keep going until we either process the whole original list or we go through the entire list
    // without finding anything
    auto newlist = std::make_unique<QList<Node>>();
    newlist->append(this->boundaryNodes->takeFirst());
    while(this->boundaryNodes->length() != 0){ // we exit early if we get a partition of the set
        bool fullCycle = true;
        for(Node n: *(this->boundaryNodes)){
            if(this->edges->value(newlist->first()).contains(n)){
                this->boundaryNodes->removeOne(n);
                newlist->prepend(n);
                fullCycle = false;
            }
            else if(this->edges->value(newlist->last()).contains(n)){
                this->boundaryNodes->removeOne(n);
                newlist->append(n);
                fullCycle = false;
            }
        }
        if(fullCycle) return; //boundary set does not form a single ring.
    }
    //we've sorted the whole boundary and can calim so
    this->boundaryNodes = std::move(newlist);
    this->is_boundary_sorted = true;

}

void Circles::Graph::Graph::sortNeighbours(Node n)
{
    QList<Node> oldlist(this->edges->value(n));
    QList<Node> newlist;
    newlist.append(oldlist.takeFirst());
    while(oldlist.length() != 0){
        bool fullCycle = true;
        for(Node m: oldlist){
            if(this->edges->value(newlist.first()).contains(m)){
                oldlist.removeOne(m);
                newlist.prepend(m);
                fullCycle = false;
            }
            else if(this->edges->value(newlist.last()).contains(m)){
                oldlist.removeOne(m);
                newlist.append(m);
                fullCycle = false;
            }
        }
        if(fullCycle) return; //boundary set does not form a single ring.
    }
    this->edges->insert(n, newlist);
    this->is_edges_sorted->insert(n, true);
}


bool Circles::Graph::operator==(const Graph& lhs, const Graph& rhs)
{
    bool e =  *(lhs.edges) == *(rhs.edges);
    bool n = (lhs.getNodes() == rhs.getNodes());
    return e && n;
}

bool operator!=(const Graph& lhs, const Graph& rhs)
{
    return !(lhs == rhs);
}
