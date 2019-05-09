#include <vector>
#include <gtest/gtest.h>
#include <iostream>
#include <ngl/Vec3.h>

#include "Graph.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Graph, defaultctor)
{
    Graph g;
    EXPECT_TRUE(g.size() == 0);
    EXPECT_TRUE(g.degree() == 3);
    EXPECT_TRUE(g.edges(0).size() == 0);
    EXPECT_TRUE(g.edges(6).size() == 0);
}

TEST(Graph, userctor)
{
    // allocate initializer list
    std::vector<ngl::Vec3> points;
    points.reserve(16);
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            points.push_back(ngl::Vec3(1.0f * i, 1.0f * j, 0.0f));
        }
    }
    // feed to graph
    Graph g(points);
    EXPECT_TRUE(g.size() == 16);
    // detect edge correctness
    auto zedges = g.edges(0);
    EXPECT_TRUE(zedges.size() == 3);
    EXPECT_TRUE(zedges[0] == 1);
    EXPECT_TRUE(zedges[1] == 4);
    EXPECT_TRUE(zedges[2] == 5);
    auto oedges = g.edges(5);
    EXPECT_TRUE(oedges.size() == 5);
    EXPECT_TRUE(oedges[0] == 1);
    EXPECT_TRUE(oedges[1] == 4);
    EXPECT_TRUE(oedges[2] == 6);
    EXPECT_TRUE(oedges[3] == 0);
    EXPECT_TRUE(oedges[4] == 9);
}

TEST(Graph, render)
{
    // initialize graph
    std::vector<ngl::Vec3> points;
    points.reserve(16);
    for(size_t i = 0; i < 4; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
        {
            points.push_back(ngl::Vec3(1.0f * i, 1.0f * j, 0.0f));
        }
    }
    Graph g(points);
    // render
    auto lines = g.render();
    EXPECT_TRUE(lines.size() == 112);
}
