#include "gtest/gtest.h"

#include "graph/Edge.hpp"

using Circles::Graph::Edge;

TEST(testEdge, edgesAreOrdered)
{
    Edge e(1, 2);
    EXPECT_LT(e.getX(), e.getY());
    e.setX(4);
    EXPECT_LT(e.getX(), e.getY());

    Edge f(2, 1);
    EXPECT_LT(e.getX(), e.getY());

}
