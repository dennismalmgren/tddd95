// algo_lib.h
// Author: Dennis Malmgren
// Helper file with multiple useful
// classes and methods for algorithmic problem solving.


#ifndef __ALGO_LIB_H
#define __ALGO_LIB_H
#include <bits/stdc++.h>

namespace algo
{

/// @brief Class that contains information about edges
/// in a graph. Keeps track of the cost (weight) to transition 
/// the edge as well as the source and target nodes it connects.
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

/// @brief Helper class to represent nodes in a graph.
struct Node 
{
    Node() = default;
    int n;
    int weight;
};

template<typename T>
using base_adjacency_graph = std::vector<std::vector<BaseEdge<T>>>;

using adjacency_graph = base_adjacency_graph<int>;
using double_adjacency_graph = base_adjacency_graph<double>;

const int MAX_DIST = std::numeric_limits<int>::max();

/// @brief Depth-first search algorithm that only allows
/// traversal of edges with a positive capacity. Complexity O(V+E).
/// @param graph an adjacency list representation of a graph
/// @param parents pre-allocated vector to store parent indices in.
/// @param capacity the capacity for each node-node pair.
/// @param start the node to identify paths from
/// @param stop the target to reach, after which the algorithm ends.
/// @return true if the stop node could be reached.
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

/// @brief Breadth-first search algorithm that only allows
/// traversal of edges with a positive capacity. Complexity O(E).
/// @param graph an adjacency list representation of a graph
/// @param parents pre-allocated vector to store parent indices in.
/// @param capacity the capacity for each node-node pair.
/// @param visited pre-allocated vector to mark visited nodes in.
/// @param start the node to identify paths from
/// @param stop the target to reach, after which the algorithm ends.
/// @return true if the stop node could be reached.
bool bfs_flow(const adjacency_graph& graph, 
                std::vector<int>& parents,
                std::vector<std::vector<int>>& capacity,
                    std::vector<bool>& visited,
                        int start, 
                        int stop) 
{
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

/// @brief Algorithm to solve the max-flow problem. Implements the
/// Ford-Fulkerson method with breadth-first search. 
/// Complexity O(EF) where F is the maximum flow of the network.
/// @param g an adjacency list representation of a graph
/// @param capacity the capacity for each node-node pair.
/// @param flow the current flow between each node-node pair.
/// @param s Source node.
/// @param t Sink node.
/// @return The maximum flow.
int max_flow(adjacency_graph& g, 
                std::vector<std::vector<int>>& capacity, 
                std::vector<std::vector<int>>& flow, 
                int s, int t) 
{
    std::vector<int> parents(g.size(), -2);
    std::vector<bool> visited(g.size(), false);
    int the_flow = 0;
    while (bfs_flow(g, parents, capacity, visited, s, t)) {
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
        visited = std::vector<bool>(g.size(), false);
    }
    return the_flow;
}


/// @brief Class that contains information about nodes
/// in a graph. Keeps track of distance estimates to the node.
template<typename T>
struct DistNode
{
    DistNode() = default;
    DistNode(int n, T dist) : n(n), dist(dist) {}
    int n;
    T dist;
};


/// @brief Identifies the shortest paths from a source node 
/// to each node in a graph using Djikstra's algorithm. 
/// The function uses O((V+E)logV) time where V is the vertex count
/// and E is the edge count.
/// @param graph an adjacency list representation of a graph
/// @param start the node to identify paths from
/// @return A pair where the first element is the distances to each node identified
/// by their position in the list, and the second element is the immediate parent of each
/// node in the graph, when following the shortest path to that node.
template<typename T>
std::pair<std::vector<T>, std::map<int, int>> 
shortest_path(base_adjacency_graph<T>& graph, int start)
{
    const T MAX_DIST = std::numeric_limits<T>::max();
    int n = graph.size();
    std::vector<T> distances(n, MAX_DIST);
    std::map<int, int> parents;

    auto comp = [](const DistNode<T>& a, const DistNode<T>& b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else {
            return a.n < b.n;
        }
    };

    std::set<DistNode<T>, decltype(comp)> unvisited(comp);

    DistNode<T> root;
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
                    DistNode<T> searchNode;
                    searchNode.dist = distances[edge.target];
                    searchNode.n = edge.target;
                    unvisited.erase(searchNode);
                }
                distances[edge.target] = distances[node.n] + edge.weight;
                DistNode<T> unvisitedNode;
                unvisitedNode.dist = distances[edge.target];
                unvisitedNode.n = edge.target;
                unvisited.insert(unvisitedNode);
                parents[edge.target] = node.n;
            }
        }
    }

    return std::make_pair(distances, parents);
}

using ll = long long;

/// @brief Calculates the shortest path 
/// while optimizing for cost. Only traverses edges where capacity is 
/// positive, and uses forward and backward edges in its search.
/// Assumes costs are positive.
/// Complexity O((V+E)log V) as it uses Djikstra's algorithm.
/// @param graph an adjacency list representation of a graph
/// @param reversed same as graph but with edges reversed.
/// @param start the node to identify paths from
/// @param potentials the current best distances to each node.
/// @param capacity the capacity for each node-node pair.
/// @param flow the current flow between each node-node pair.
/// @param cost the cost for each edge in the graph.
/// @param parents pre-allocated vector in which to mark
/// the parents in a shortest path tree.
/// @param visited pre-allocated vector in which to mark 
/// visited nodes during search.
/// @param distances pre-allocated vector in which to save 
/// shortest distances encountered during search.
void shortest_path_flow(adjacency_graph& graph, 
        adjacency_graph& reversed,  
        int start,
        std::vector<ll>& potentials,
        std::vector<std::vector<ll>>& capacity,
        std::vector<std::vector<ll>>& flow,
        std::vector<std::vector<ll>>& cost,
        std::vector<std::pair<int, bool>>& parents,
        std::vector<bool>& visited,
        std::vector<ll>& distances)
{
    const ll MAX_DIST = std::numeric_limits<ll>::max() / 2;
    std::fill(distances.begin(), distances.end(), MAX_DIST);
    std::fill(visited.begin(), visited.end(), false);
    int n = graph.size();
    auto comp = [](const DistNode<ll>& a, const DistNode<ll>& b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist;     
        }
        else {
            return a.n < b.n;
        }
    };

    std::set<DistNode<ll>, decltype(comp)> unvisited(comp);
    unvisited.insert(DistNode<ll>(start, 0));
    distances[start] = 0;
    while (unvisited.size() > 0) {
        auto node = unvisited.begin()->n;
        unvisited.erase(unvisited.begin());
        visited[node] = true;
        int node_dist = distances[node] + potentials[node];
        for (auto& edge: graph[node]) {  
            if (!visited[edge.target]) {
                auto target_dist = node_dist - potentials[edge.target] + cost[edge.source][edge.target];
                if (capacity[edge.source][edge.target] - flow[edge.source][edge.target] > 0 && target_dist < distances[edge.target]) {
                    if (distances[edge.target] != MAX_DIST) {
                        unvisited.erase(DistNode<ll>(edge.target, distances[edge.target]));
                    }
                    distances[edge.target] = target_dist;
                    parents[edge.target] = std::make_pair(edge.source, true);
                    unvisited.insert(DistNode<ll>(edge.target, distances[edge.target]));
                }  
            }
        }

        for (auto& edge: reversed[node]) {  
            if (!visited[edge.target]) {
                auto target_dist = node_dist - potentials[edge.target] - cost[edge.target][edge.source];
                if (flow[edge.target][edge.source] > 0 && target_dist < distances[edge.target]) {
                    if (distances[edge.target] != MAX_DIST) {
                        unvisited.erase(DistNode<ll>(edge.target, distances[edge.target]));
                    }
                    distances[edge.target] = target_dist;
                    parents[edge.target] = std::make_pair(edge.source, false);
                    unvisited.insert(DistNode<ll>(edge.target, distances[edge.target]));
                }  
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        potentials[i] = std::min(distances[i] + potentials[i], MAX_DIST);
    }
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

/// @brief Algorithm to calculate the minimum spanning tree. 
/// Time complexity: O(E log E)
/// @tparam T the type of values in the graph
/// @param graph adjacency list representation of a graph.
/// @return std::nullopt if there is no MST. Otherwise a pair, 
/// where the first element is the total weight of the tree and the second
/// element is the edges of the tree.
template<typename T>
std::optional<std::pair<T, std::vector<BaseEdge<T>>>> 
mst(base_adjacency_graph<T>& graph) {
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

/// @brief  Helper function that returns a vector of indices ordered
/// according to the values of the input vector. Time complexity: O(n log n)
/// @tparam T the type of elements in the vector.
/// @param array the vector of elements the indices should be sorted according to.
/// @return a vector of indices in sorted order.
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

/// @brief Algorithm to identify all nodes in a strongly connected component in a graph,
/// starting from a specific node. O(E).
/// @param i The node to investigate.
/// @param scc The resulting set of nodes in the component.
/// @param scc_id the scc id allocated to the current scc.
/// @param scc_ids the map from node id to scc id.
/// @param discovered mark nodes as discovered during search.
/// @param adjacency adjacency list representation of a graph.
void get_scc(int i, 
            std::set<int>& scc, 
            int scc_id, 
            std::map<int, int>& scc_ids, 
            std::vector<bool>& discovered,
            adjacency_graph& adjacency) 
{
    discovered[i] = true;
    scc.insert(i);
    scc_ids[i] = scc_id;
    for (auto edge: adjacency[i]) {
        if (!discovered[edge.target]) {
            get_scc(edge.target, scc, scc_id, scc_ids, discovered, adjacency);
        }
    }
}

/// @brief Identifies the shortest paths in a graph using the Bellman-Ford method.
/// It can handle negative weights and negative cycles. Time complexity: O(VE).
/// @tparam T Type element in the graph.
/// @param n The number of nodes.
/// @param edges Edge list representation of the graph.
/// @param distances Preallocated list of distances to each node.
/// @param parents Preallocated list to save the parents of each node
/// when travelling along the shortest paths.
/// @param start Start node to find paths from
/// @param ignore_cycles True if negative cycles should be ignored.
template<typename T>
void shortest_path_negative_weights(int n, 
                                    std::vector<BaseEdge<T>>& edges, 
                                    std::vector<T>& distances,
                                    std::vector<T>& parents,
                                    int start, 
                                    bool ignore_cycles = false)
{
    const T MAX_DIST = std::numeric_limits<T>::max();
    const T MIN_DIST = std::numeric_limits<T>::min();
    
    distances[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (auto& edge: edges) {
            if (distances[edge.source] == MAX_DIST) {
                continue;
            }
            if (distances[edge.source] + edge.weight < distances[edge.target]) {
                distances[edge.target] = distances[edge.source] + edge.weight;
                parents[edge.target] = edge.source;
            }
        }
    }
    
    // Run it again..
    if (!ignore_cycles) {
        for (int i = 0; i < n - 1; i++) {
            for (auto& edge: edges) {
                if (distances[edge.source] < MAX_DIST) {
                    if (distances[edge.source] == MIN_DIST) {
                        distances[edge.target] = MIN_DIST;
                        parents[edge.target] = edge.source;
                    }
                    else if (distances[edge.source] + edge.weight < distances[edge.target]) {
                        distances[edge.target] = MIN_DIST;
                        parents[edge.target] = edge.source;
                    }
                }
            }
        }
    }
}

/// @brief Solves the min-cost-max-flow problem using the Successive Shortest Paths
/// method with potentials. Time complexity: O(V E F log V) where F is the maximum flow.
/// @param n The number of nodes.
/// @param graph Adjacency list representation of the graph.
/// @param reversed The same graph but with edges reversed.
/// @param capacity The capacity for each node-node pair.
/// @param flow The current flow between each node-node pair.
/// @param cost The unit cost for flow between each node-node pair.
/// @param s The source node.
/// @param t The sink node.
/// @return A pair where the first element is the cost and the second element is the
/// flow.
std::pair<ll, ll> min_cost_max_flow(int n, 
                adjacency_graph& graph, 
                adjacency_graph& reversed, 
                std::vector<std::vector<ll>>& capacity, 
                std::vector<std::vector<ll>>& flow, 
                std::vector<std::vector<ll>>& cost, 
                int s, int t) 
{
    const ll MAX_DIST = std::numeric_limits<ll>::max() / 2;
    ll the_flow = 0;
    ll the_cost = 0;
    std::vector<std::pair<int, bool>> parents(n);
    std::vector<ll> distances(n);
    std::vector<ll> potentials(n, 0);
    std::vector<bool> visited(n, false);

    while (true) {
        shortest_path_flow(graph, reversed, s, potentials, capacity, flow, cost, parents, visited, distances); 

        if (distances[t] == MAX_DIST || !visited[t]) {
            break;
        }

        ll path_flow = MAX_DIST;
        bool forward;
        int u;
        // u = p, v = x.
        // r refers to parent.
        for (auto v = t; v != s; v = parents[v].first) {
            u = parents[v].first;
            forward = parents[v].second;

            if (forward) {
                path_flow = std::min(path_flow, capacity[u][v] - flow[u][v]);
            }
            else {
                path_flow = std::min(path_flow, flow[v][u]);
            }
        }

        the_flow += path_flow;

        for (auto v = t; v != s; v = parents[v].first) {
            u = parents[v].first;
            forward = parents[v].second;

            if (forward) {
                flow[u][v] += path_flow;
            }
            else {
                flow[v][u] -= path_flow;
            }
        }
    }
       
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            the_cost += cost[i][j] * flow[i][j];
        }
    }
    return std::make_pair(the_cost, the_flow);
}

}

#endif
