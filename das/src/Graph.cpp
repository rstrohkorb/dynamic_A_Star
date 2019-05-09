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
    // now add reverse edges to make sure we're all bidirectional in this graph
    for(size_t n = 0; n < m_graph.size(); ++n)
    {
        // loop through neighbor list and make sure we're in their neighbor list
        for(auto e : m_graph[n].es)
        {
            // grab edgeID list
            auto nEdges = m_graph[e.n].edgeId();
            // add ourselves if we're not there
            if(std::find(nEdges.begin(), nEdges.end(), n) == nEdges.end())
            {
                Edge newEdge(n, (_points[n] - _points[e.n]).lengthSquared());
                m_graph[e.n].es.push_back(newEdge);
            }
        }
    }
}

std::vector<size_t> Graph::edges(const size_t _node) const
{
    std::vector<size_t> edg;
    if(_node < m_graph.size())
    {
        edg = m_graph[_node].edgeId();
    }
    return edg;
}

std::vector<ngl::Vec3> Graph::render() const
{
    std::vector<ngl::Vec3> lines;
    // Loop through and dump everything
    // For now, we're not going to try to avoid bidirectional duplicates
    for(size_t n = 0; n < m_graph.size(); ++n)
    {
        for(auto e : m_graph[n].es)
        {
            lines.push_back(m_graph[n].p);
            lines.push_back(m_graph[e.n].p);
        }
    }
    return lines;
}

size_t Graph::find_index(std::vector<float> _list, float _item, std::vector<size_t> _eId) const
{
    for(size_t i = 0; i < _list.size(); ++i)
    {
        if(FCompare(_list[i], _item))
        {
            // Protect against same length
            if(std::find(_eId.begin(), _eId.end(), i) == _eId.end())
            {
                return i;
            }
        }
    }
    // out of index if not present (shouldn't happen)
    return _list.size();
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
