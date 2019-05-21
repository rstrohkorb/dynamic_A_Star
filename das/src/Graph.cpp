#include <utility>
#include <algorithm>
#include <queue>
#include <iostream>
#include <ngl/Vec3.h>
#include "Graph.h"

Graph::Graph(std::vector<ngl::Vec3> _points, size_t _degree) : m_degree(_degree)
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
        for(size_t wi = 1; wi < m_degree + 1; ++wi)
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

bool Graph::isEdge(size_t _n1, size_t _n2)
{
    // Assumes bidirectional completeness - doesn't check n2's edges
    if(_n1 < m_graph.size() && _n2 < m_graph.size())
    {
        Edge en2(_n2, 0.0f);
        if(std::find(m_graph[_n1].es.begin(), m_graph[_n1].es.end(), en2) != m_graph[_n1].es.end())
        {
            return true;
        }
    }
    return false;
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

void Graph::removeEdge(size_t _n1, size_t _n2)
{
    if(this->isEdge(_n1, _n2))
    {
        // Go through _n1's edge list and remove _n2
        Edge en2(_n2, 0.0f);
        auto posn2 = std::find(m_graph[_n1].es.begin(), m_graph[_n1].es.end(), en2);
        m_graph[_n1].es.erase(posn2);
        // Go through _n2's edge list and remove _n1
        Edge en1(_n1, 0.0f);
        auto posn1 = std::find(m_graph[_n2].es.begin(), m_graph[_n2].es.end(), en1);
        m_graph[_n2].es.erase(posn1);
    }
}

std::vector<ngl::Vec3> Graph::aStar(size_t _self, size_t _goal)
{
    float initVal = 1000.0f;
    // node you came from, currently most effective
    std::vector<size_t> cameFrom(m_graph.size(), m_graph.size());
    cameFrom[_self] = _self;
    // gscore - for each node, cost of getting from start to the node
    std::vector<float> gscore(m_graph.size(), initVal);
    gscore[_self] = 0.0f;
    // fscore - cost of getting from start to goal through this node
    std::vector<float> fscore(m_graph.size(), initVal);
    fscore[_self] = heuristic_cost_estimate(_self, _goal);
    // open priority queue for processing nodes
    std::priority_queue<ScoreSort, std::vector<ScoreSort>, std::greater<ScoreSort>> open;
    ScoreSort selfscore(_self, fscore[_self]);
    open.push(selfscore);

    // loop
    while(!open.empty())
    {
        auto current = open.top();
        // if we've reached the goal, stop the loop
        if(current.n == _goal)
        {
            return reconstructPath(cameFrom, current.n);
        }
        // remove top value - we've processed
        open.pop();
        // if this node's fscore doesn't match what's in the list, discard
        if(!(FCompare(current.fscore, fscore[current.n])))
        {
            continue;
        }

        // loop through current's neighbors and add to open
        for(auto e : m_graph[current.n].es)
        {
            // tentative distance measurement between us and neighbor
            auto temp_gscore = gscore[current.n] + e.w;

            // don't do anything if our neighbor's gscore is already better
            if((temp_gscore > gscore[e.n]) || FCompare(temp_gscore, gscore[e.n]))
            {
                continue;
            }

            // update values, since this is currently the best path
            cameFrom[e.n] = current.n;
            gscore[e.n] = temp_gscore;
            fscore[e.n] = gscore[e.n] + heuristic_cost_estimate(e.n, _goal);

            // add neighbor to open set
            ScoreSort ss(e.n, fscore[e.n]);
            open.push(ss);
        }
    }

    // we should never reach this line
    return reconstructPath(cameFrom, _goal);
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

float Graph::heuristic_cost_estimate(size_t _self, size_t _goal)
{
    // distance between the two nodes
    return (m_graph[_goal].p - m_graph[_self].p).length();
}

std::vector<ngl::Vec3> Graph::reconstructPath(std::vector<size_t> _cameFrom, size_t _current)
{
    std::vector<ngl::Vec3> path;
    auto ncf = _current;
    while(_cameFrom[ncf] != ncf)
    {
        // insert position
        path.insert(path.begin(), m_graph[ncf].p);
        // update ncf
        ncf = _cameFrom[ncf];
    }
    return path;
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
