#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <ngl/Vec3.h>



class Graph
{
public:
    Graph()=default;
    Graph(std::vector<ngl::Vec3> _points, size_t _degree);

    size_t size() const { return m_graph.size(); }          // returns number of nodes in graph
    size_t degree() const { return m_degree; }              // returns minimum degree of graph
    ngl::Vec3 pos(const size_t _node) const
            { return m_graph[_node].p; }                    // returns position of the input node
    size_t node(const ngl::Vec3 _pos) const;                // returns node value given the input position
    std::vector<size_t> edges(const size_t _node) const;    // returns edges connected to the input node
    bool isEdge(size_t _n1, size_t _n2);                    // returns true if there is an edge between the input nodes
    std::vector<ngl::Vec3> render() const;                  // returns list of positions for GL_LINES

    void removeEdge(size_t _n1, size_t _n2);                // removes the edge between the two provided notes

    std::vector<ngl::Vec3> aStar(size_t _self, size_t _goal);   // runs astar algorithm between given indices

private:
    // Private struct Edge, for keeping track of weights
    struct Edge
    {
        size_t n; // neighbor id value
        float w;  // weight of this edge

        // Constructor
        Edge(size_t _n, float _w) : n(_n), w(_w) {;}
        // Operator ==
        bool operator==(const Edge& _other) const { return this->n == _other.n;}
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
    // Private struct ScoreSort, for sorting by fscore in the aStar priority queue
    struct ScoreSort
    {
        size_t n; // node id value
        float fscore; // node fscore value

        // Constructor
        ScoreSort(size_t _n, float _fscore) : n(_n), fscore(_fscore) {;}
        // Operator overrides - find values of same n
        bool operator==(const ScoreSort& _other) const { return this->n == _other.n; }
        bool operator!=(const ScoreSort& _other) const { return this->n != _other.n; }
        // Operator overrides - sorting in priority queue
        bool operator<(const ScoreSort& _other) const { return this->fscore < _other.fscore; }
        bool operator<=(const ScoreSort& _other) const { return (FCompare(this->fscore, _other.fscore) || (this->fscore < _other.fscore)); }
        bool operator>(const ScoreSort& _other) const { return this->fscore > _other.fscore; }
        bool operator>=(const ScoreSort& _other) const { return (FCompare(this->fscore, _other.fscore) || (this->fscore > _other.fscore)); }
    };

    // MEMBER VARIABLES
    std::vector<Node> m_graph;
    size_t m_degree = 3;

    // PRIVATE FUNCTIONS
    size_t find_index(std::vector<float> _list, float _item, std::vector<size_t> _eId) const;
    float heuristic_cost_estimate(size_t _self, size_t _goal);
    std::vector<ngl::Vec3> reconstructPath(std::vector<size_t> _cameFrom, size_t _current);
};

#endif
