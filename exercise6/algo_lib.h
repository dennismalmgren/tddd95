#ifndef __ALGO_LIB_H
#define __ALGO_LIB_H
#include <bits/stdc++.h>

namespace algo
{

template<typename T>
struct BaseEdge
{
    BaseEdge() = default;
    BaseEdge(const BaseEdge& rhs) = default;

    BaseEdge(int source, int target, T weight) 
    : source(source), target(target), weight(weight)
    {

    }

    int source;
    int target;
    T weight;
};

using Edge = BaseEdge<int>;
using DoubleEdge = BaseEdge<double>;

struct Node 
{
    Node() = default;
    int n;
};

template<typename T>
using base_adjacency_graph = std::vector<std::vector<BaseEdge<T>>>;

using adjacency_graph = base_adjacency_graph<int>;
using double_adjacency_graph = base_adjacency_graph<double>;

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

struct DistNode
{
    int n;
    int dist;
};

std::pair<std::vector<int>, std::map<int, int>> shortest_path(adjacency_graph& graph, int start)
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    int n = graph.size();
    std::vector<int> distances(n, MAX_DIST);
    std::map<int, int> parents;

    auto comp = [](DistNode a, DistNode b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else {
            return a.n < b.n;
        }
    };

    //std::priority_queue<Node, std::vector<Node>, decltype(comp)> unvisited(comp);
    std::set<DistNode, decltype(comp)> unvisited(comp);

    DistNode root;
    root.dist = 0;
    root.n = start;
    unvisited.insert(root);
    distances[start] = 0;

    while (unvisited.size() > 0) {
        auto node = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        for (auto edge: graph[node.n]) {    
            if (distances[edge.target] > distances[node.n] + edge.weight) {
                if (distances[edge.target] != MAX_DIST) {
                    DistNode searchNode;
                    searchNode.dist = distances[edge.target];
                    searchNode.n = edge.target;
                    unvisited.erase(searchNode);
                }
                distances[edge.target] = distances[node.n] + edge.weight;
                DistNode unvisitedNode;
                unvisitedNode.dist = distances[edge.target];
                unvisitedNode.n = edge.target;
                unvisited.insert(unvisitedNode);
                parents[edge.target] = node.n;
            }
        }
    }

    return std::make_pair(distances, parents);
}

/// @brief Helper class to implement disjoint sets.
/// supports union and find operations.
class DisjointSet {
    public:
        /// @brief Constructor. Creates an initial structure
        /// with all elements in individual sets.
        /// @param n the number of elements in the set.
        DisjointSet(int n) : n(n), parent(n), rank(n) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        
        /// @brief Finds the root of the set for an element.
        /// also performs path compression during the find, as a side-effect.
        /// Executes in O(log n)
        /// @param elem the element to find the root for.
        /// @return the root of the element. 
        int find_set(int elem) {
            if (elem == parent[elem]) {
                return elem;
            }
            return parent[elem] = find_set(parent[elem]);
        }

        /// @brief Merges the sets of two elements.
        /// uses the rank of the individual sets to optimize the depth
        /// of the resulting tree structure.
        /// Executes in O(log n)
        /// @param elem1 Element one.
        /// @param elem2  Element two.
        void union_sets(int elem1, int elem2) {
            auto a = find_set(elem1);
            auto b = find_set(elem2);
            if (a != b) {
                if (rank[a] < rank[b]) {
                    std::swap(a, b);
                }
                parent[b] = a;
                if (rank[a] == rank[b]) {
                    rank[a]++;
                }
            }
        }

    private:
        int n;
        std::vector<int> parent;
        std::vector<int> rank;
};


template<typename T>
std::optional<std::pair<T, std::vector<BaseEdge<T>>>> mst(base_adjacency_graph<T>& graph) {
    std::pair<T, std::vector<BaseEdge<T>>> result;
    DisjointSet disjointSet(graph.size());
    std::vector<BaseEdge<T>> all_edges;

    for (auto& node_vec : graph) {
        for (auto& edge : node_vec) {
            all_edges.push_back(edge);
        }
    }
    std::sort(all_edges.begin(), all_edges.end(), [](BaseEdge<T>& e1, BaseEdge<T>& e2) { return e1.weight < e2.weight;});
    int edge_cost = 0;
    for (auto edge: all_edges) {
        if (disjointSet.find_set(edge.source) != disjointSet.find_set(edge.target)) {
            result.second.push_back(edge);
            disjointSet.union_sets(edge.source, edge.target);
            result.first += edge.weight;
        }
    }
    std::sort(result.second.begin(), result.second.end(), [](BaseEdge<T>& e1, BaseEdge<T>& e2) {
        if (e1.source != e2.source) {
            return e1.source < e2.source;
        }
        return e1.target < e2.target;
    });

    int root_set = disjointSet.find_set(0);
    for (int i = 1; i < graph.size(); i++) {
        if (root_set != disjointSet.find_set(i)) {
            return std::nullopt;
        }
    }

    return result;
}

template<typename T>
std::vector<size_t> argsort(const std::vector<T> &array) {
    std::vector<size_t> indices(array.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [&array](int left, int right) -> bool {
                  // sort indices according to corresponding array element
                  return array[left - 1] < array[right - 1];
              });

    return indices;
}

}

#endif