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

// std::pair<std::vector<int>, int> 
//         bfs_flow(FlowAdjacencyGraph graph, 
//                         int start, 
//                         int stop) 
// {
//     std::vector<int> parents(graph.size(), -1);
//     parents[start] = -2;
//     std::queue<std::pair<int, int>> q;
//     q.push(std::make_pair(start, MAX_DIST));
//     while (!q.empty()) {
//         auto[current_node, current_flow] = q.front();
//         q.pop();
//         for (auto& edge: graph[current_node]) {
//             if (parents[edge.target] == -1 && edge.weight > 0) {
//                 parents[edge.target] = current_node;
//                 int new_flow = std::min(current_flow, edge.weight);
//                 if (edge.target == stop) {
//                     return std::make_pair(parents, new_flow);
//                 }
//                 q.push(std::make_pair(edge.target, new_flow));
//             }
//         }
//     }
//     return std::make_pair(parents, 0);
// }

// std::pair<std::vector<int>, std::map<int, int>> 
//         shortest_path(FlowAdjacencyGraph graph, 
//                         int start, 
//                         int stop = -1,
//                         bool useweightsstrictlyabovezero=false)
// {
//     std::vector<int> distances(graph.size(), MAX_DIST);
//     std::map<int, int> parents;

//     auto comp = [](DistNode a, DistNode b ) { 
//         if (a.payload.dist != b.payload.dist) {
//             return a.payload.dist < b.payload.dist; 
//         }
//         else {
//             return a.n < b.n;
//         }
//     };

//     std::set<DistNode, decltype(comp)> unvisited(comp);

//     DistNode root;
//     root.payload.dist = 0;
//     root.n = start;
//     unvisited.insert(root);
//     distances[start] = 0;

//     while (unvisited.size() > 0) {
//         auto node = *unvisited.begin();
//         unvisited.erase(unvisited.begin());
//         if (node.n == stop) {
//             break;
//         }
//         for (auto edge: graph[node.n]) {    
//             if ((!useweightsstrictlyabovezero || edge.weight > 0) 
//                         && distances[edge.target] > distances[node.n] + edge.weight) {
//                 if (distances[edge.target] != MAX_DIST) {
//                     DistNode searchNode;
//                     searchNode.payload.dist = distances[edge.target];
//                     searchNode.n = edge.target;
//                     unvisited.erase(searchNode);
//                 }
//                 distances[edge.target] = distances[node.n] + edge.weight;

//                 DistNode unvisitedNode;
//                 unvisitedNode.payload.dist = distances[edge.target];
//                 unvisitedNode.n = edge.target;
//                 unvisited.insert(unvisitedNode);
//                 parents[edge.target] = node.n;
//             }
//         }
//     }

//     return std::make_pair(distances, parents);
// }

}

#endif