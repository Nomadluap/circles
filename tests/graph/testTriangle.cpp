#include "gtest/gtest.h"

#include "graph/Triangle.hpp"

using Circles::Graph::Triangle;

TEST(testTriangle, assignment)
{
    Triangle a {1, 2, 3};
    Triangle b = a;
    ASSERT_TRUE(a == b);
}
