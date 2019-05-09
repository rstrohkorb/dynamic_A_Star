#include <utility>
#include <algorithm>
#include <ngl/Vec3.h>
#include "Graph.h"

Graph::Graph(std::vector<ngl::Vec3> _points)
{
    // allocate graph
    m_graph.reserve(_points.size());
    for(auto point : _points)
    {
        Node n(point);
        m_graph.push_back(n);
    }
    // add edges
    for(size_t n = 0; n < m_graph.size(); ++n)
    {
        std::vector<float> weights;
        weights.reserve(_points.size());
        // calc and store distance between self and every other node
        for(size_t i = 0; i < _points.size(); ++i)
        {
            weights.push_back((_points[n] - _points[i]).lengthSquared());
        }
        // copy to another list and sort
        std::vector<float> sorted_weights(weights);
        std::sort(sorted_weights.begin(), sorted_weights.end());
        // locate sorted element and store - skip first one, that's us
        for(size_t wi = 1; wi < 4; ++wi)
        {
            Edge e(find_index(weights, sorted_weights[wi], m_graph[n].edgeId()), sorted_weights[wi]);
            m_graph[n].es.push_back(e);
        }
    }
}

std::vector<size_t> Graph::edges(size_t node) const
{
    std::vector<size_t> edg;
    if(node < m_graph.size())
    {
        for(auto e : m_graph[node].es)
        {
            edg.push_back(e.n);
        }
    }
    return edg;
}

size_t Graph::find_index(std::vector<float> list, float item, std::vector<size_t> eId) const
{
    for(size_t i = 0; i < list.size(); ++i)
    {
        if(FCompare(list[i], item))
        {
            // Protect against same length
            if(std::find(eId.begin(), eId.end(), i) == eId.end())
            {
                return i;
            }
        }
    }
    // out of index if not present (shouldn't happen)
    return list.size();
}

std::vector<size_t> Graph::Node::edgeId() const
{
    std::vector<size_t> eId;
    for(auto e : this->es)
    {
        eId.push_back(e.n);
    }
    return eId;
}
