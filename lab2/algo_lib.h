#ifndef __ALGO_LIB_H
#define __ALGO_LIB_H
#include <bits/stdc++.h>

namespace algo
{


struct Edge
{

    Edge() = default;
    Edge(const Edge& rhs) = default;

    Edge(int source, int target, int weight) 
    : source(source), target(target), weight(weight)
    {

    }

    int source;
    int target;
    int weight;
};

template<typename T>
struct Node 
{
    Node() = default;
    int n;
};

using adjacency_graph = std::vector<std::vector<Edge>>;

const int MAX_DIST = std::numeric_limits<int>::max();

bool dfs_flow(const adjacency_graph& graph, 
                std::vector<int>& parents,
                std::vector<std::vector<int>>& capacity,
                        int start, 
                        int stop) 
{
    std::vector<bool> visited(graph.size(), false);
    parents[start] = -1;
    std::stack<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int current_node = q.top();
        q.pop();
        for (auto& edge: graph[current_node]) {
            if (!visited[edge.target] && capacity[edge.source][edge.target] > 0) {
                q.push(current_node);
                visited[edge.target] = true;
                q.push(edge.target);
                parents[edge.target] = current_node;
                if (edge.target == stop) {
                    return true;
                }
            }
        }
    }
    return false;

}

bool bfs_flow(const adjacency_graph& graph, 
                std::vector<int>& parents,
                std::vector<std::vector<int>>& capacity,
                        int start, 
                        int stop) 
{
    std::vector<bool> visited(graph.size(), false);
    parents[start] = -1;
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int current_node = q.front();
        q.pop();
        for (auto& edge: graph[current_node]) {
            if (visited[edge.target] == false && capacity[edge.source][edge.target] > 0) {
                parents[edge.target] = current_node;
                visited[edge.target] = true;

                q.push(edge.target);

                if (edge.target == stop) {
                    return true;
                }
            }
        }
    }
    return false;

}

int max_flow(adjacency_graph& g, 
                std::vector<std::vector<int>>& capacity, 
                std::vector<std::vector<int>>& flow, 
                int s, int t) 
{
    std::vector<int> parents(g.size(), -2);
    int the_flow = 0;
    while (bfs_flow(g, parents, capacity, s, t)) {
        int path_flow = MAX_DIST;
        for (auto v = t; v != s; v = parents[v]) {
            auto u = parents[v];
            path_flow = std::min(path_flow, capacity[u][v]);
        }

        for (auto v = t; v != s; v = parents[v]) {
            auto u = parents[v];
            flow[u][v] += path_flow;
            flow[v][u] -= path_flow;                
            capacity[u][v] -= path_flow;
            capacity[v][u] += path_flow;
        }
        the_flow += path_flow;
    }
    return the_flow;
}

}

#endif