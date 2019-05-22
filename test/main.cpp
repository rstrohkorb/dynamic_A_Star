#include <vector>
#include <gtest/gtest.h>
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/NGLInit.h>

#include "Graph.h"
#include "ColorTeapot.h"

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
    Graph g(points, 3);
    EXPECT_TRUE(g.size() == 16);
    EXPECT_TRUE(g.pos(0) == ngl::Vec3(0.0f));
    EXPECT_TRUE(g.pos(15) == ngl::Vec3(3.0f, 3.0f, 0.0f));
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

TEST(Graph, removeEdge)
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
    Graph g(points, 3);
    // remove some edges
    g.removeEdge(0, 5);
    EXPECT_FALSE(g.isEdge(0, 5));
    g.removeEdge(5, 6);
    EXPECT_FALSE(g.isEdge(5, 6));
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
    Graph g(points, 3);
    // render
    auto lines = g.render();
    EXPECT_TRUE(lines.size() == 112);
}

TEST(Graph, Astar)
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
    Graph g(points, 3);
    // run Astar on fully connected graph, check the path
    auto path = g.aStar(0, 15);
    EXPECT_TRUE(path.size() == 4);
    EXPECT_TRUE(path[0] == ngl::Vec3(1.0f, 1.0f, 0.0f));
    EXPECT_TRUE((path[1] == ngl::Vec3(2.0f, 1.0f, 0.0f)) ||
                path[1] == ngl::Vec3(1.0f, 2.0f, 0.0f)); // takes a different path on mac v. linux. Same distance on grid so doesn't really matter.
    EXPECT_TRUE(path[2] == ngl::Vec3(2.0f, 2.0f, 0.0f));
    EXPECT_TRUE(path[3] == ngl::Vec3(3.0f, 3.0f, 0.0f));
    // remove some edges
    g.removeEdge(10, 15);
    g.removeEdge(10, 14);
    g.removeEdge(10, 11);
    // run astar again, check the path
    auto path2 = g.aStar(0, 15);
    EXPECT_TRUE(path2.size() == 5);
    EXPECT_TRUE(path2[0] == ngl::Vec3(1.0f, 1.0f, 0.0f));
    EXPECT_TRUE(path2[1] == ngl::Vec3(2.0f, 1.0f, 0.0f));
    EXPECT_TRUE(path2[2] == ngl::Vec3(3.0f, 1.0f, 0.0f));
    EXPECT_TRUE(path2[3] == ngl::Vec3(3.0f, 2.0f, 0.0f));
    EXPECT_TRUE(path2[4] == ngl::Vec3(3.0f, 3.0f, 0.0f));
}

TEST(ColorTeapot, defaultctor)
{
    ColorTeapot ct;
    EXPECT_TRUE(ct.numTris() == 5346 * 3);
}

TEST(ColorTeapot, render)
{
    ColorTeapot ct;
    std::vector<ngl::Vec3> colors;
    colors.push_back(ngl::Vec3(0.0f));
    colors.push_back(ngl::Vec3(1.0f));
    colors.push_back(ngl::Vec3(1.0f, 0.0f, 1.0f));
    auto renderlist = ct.render(colors);
    EXPECT_TRUE(renderlist.size() == 5346*2*3);
}
