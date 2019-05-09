#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <ngl/Vec3.h>



class Graph
{
public:
    Graph()=default;
    Graph(std::vector<ngl::Vec3> _points);

    size_t size() const { return m_graph.size(); }          // returns number of nodes in graph
    size_t degree() const { return m_degree; }              // returns minimum degree of graph
    std::vector<size_t> edges(const size_t _node) const;    // returns edges connected to the input edge
    std::vector<ngl::Vec3> render() const;                  // returns list of positions for GL_LINES

private:
    // Private struct Edge, for keeping track of weights
    struct Edge
    {
        size_t n; // neighbor id value
        float w;  // weight of this edge

        // Constructor
        Edge(size_t _n, float _w) : n(_n), w(_w) {;}
    };
    // Private struct Node, for keeping track of pos and holding edges
    struct Node
    {
        ngl::Vec3 p; // position of node
        std::vector<Edge> es;      // edge set

        // Constructor
        Node(ngl::Vec3 _p) : p(_p) {;}
        // Getter
        std::vector<size_t> edgeId() const;
    };

    // MEMBER VARIABLES
    std::vector<Node> m_graph;
    size_t m_degree = 3;

    // PRIVATE FUNCTIONS
    size_t find_index(std::vector<float> _list, float _item, std::vector<size_t> _eId) const;
};

#endif
