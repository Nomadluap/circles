#include "gtest/gtest.h"

#include "graph/Graph.hpp"

using namespace Circles::Graph;

namespace {

class testGraph : public ::testing::Test {
protected:
    virtual void SetUp(){
        // 6 nodes as a single flower around node 0
        singleFlower.addEdge(1, 2);
        singleFlower.addEdge(2, 3);
        singleFlower.addEdge(3, 4);
        singleFlower.addEdge(4, 5);
        singleFlower.addEdge(5, 6);
        singleFlower.addEdge(6, 1);
        for(int i = 1; i <= 6; ++i) singleFlower.addEdge(0, i);
    }

    virtual void TearDown(){

    }

    Graph g; //an empty graph.
    Graph singleFlower; // has one flower around 0

};

TEST_F(testGraph, hasSymmetry)
{
    g.addEdge(1, 2);
    EXPECT_TRUE(g.hasEdge(1, 2));
    EXPECT_TRUE(g.hasEdge(2, 1));
}

TEST_F(testGraph, notAutoComplete)
{
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    EXPECT_TRUE(g.hasEdge(0, 1));
    EXPECT_TRUE(g.hasEdge(0, 2));
    EXPECT_FALSE(g.hasEdge(1, 2));
}

TEST_F(testGraph, singleFlowerHasFlower)
{
    EXPECT_TRUE(singleFlower.hasFullFlower(0));

}

TEST_F(testGraph, removeEdgeViolatesFlower)
{
    singleFlower.removeEdge(1, 2);
    EXPECT_FALSE(singleFlower.hasFullFlower(0));
}

TEST_F(testGraph, equalityWorks)
{
    Graph s(singleFlower);
    Graph t = singleFlower;
    EXPECT_EQ((s.getEdges()), (t.getEdges()) );
    EXPECT_EQ((s.getNodes()), (t.getNodes()) );
    EXPECT_EQ(s, t);
    s.addEdge(0, 10);
    EXPECT_FALSE(s == t);

}

TEST_F(testGraph, lastEdgeRemovesPoint)
{
    g.addEdge(1, 2);
    EXPECT_TRUE(g.hasEdge(1, 2));
    g.removeEdge(1, 2);
    EXPECT_FALSE(g.hasEdge(1, 2));
    EXPECT_FALSE(g.hasNode(1));
    EXPECT_FALSE(g.hasNode(2));
}

} // namespace
