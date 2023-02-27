#ifndef __ALGO_LIB_H
#define __ALGO_LIB_H
#include <bits/stdc++.h>

namespace algo
{
    struct Nothing
    {

    };

    struct FlowPayload
    {
        FlowPayload() = default;
        FlowPayload(int capacity, int flow, bool backward)
        : capacity(capacity), flow(flow), backward(backward)
        {

        }

        bool backward;
        int capacity;
        int flow;
    };

    template<typename T>
    struct Edge
    {
        Edge() 
        {
            source = -1;
        }

        Edge(int source, int target, int weight, T payload) 
        : source(source), target(target), weight(weight), payload(payload)
        {

        }

        bool is_dummy() 
        {
            return source == -1;
        }
        int source;
        int target;
        int weight;
        T payload;
    };


    template<typename T>
    using adjacency_graph = std::vector<std::vector<Edge<T>>>;

    template<typename T>
    using matrix_ref_graph = std::vector<std::vector<Edge<T>&>>;
}

using FlowEdge = Edge<FlowPayload>;
using FlowAdjacencyGraph = adjacency_graph<FlowPayload>;

int dfs_flow(FlowAdjacencyGraph& g, int s, int t, std::vector<FlowEdge>& augmented_path, std::vector<bool>& discovered)
{
    discovered[i] = true;
    for (auto edge: adjacency[i]) {
        if (!discovered[edge.target]) {
            dfs(edge.target, the_stack, discovered, adjacency);
        }
    }
    the_stack.push(i);
}

#endif