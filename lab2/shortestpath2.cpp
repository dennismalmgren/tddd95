
// shortestpath2.cpp
// Author: Dennis Malmgren
// Finds the single source shortest paths in a directed graph
// while handling the fact that edges are only available when
// so specified in a time table. Only works with
// integer distances.

#include "algo_lib.h"
#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_shortestpath2.txt");
//auto& cin = std::cin;


using namespace algo;
using ll = long long;


/// @brief Class that contains information about edges
/// in a graph. Keeps track of the cost (weight) to transition 
/// the edge as well as the target node it connects to.
/// The class also keeps track of when it is first available (t0)
/// and at what interval (p). p=0 indicates that the edge is only
/// available at t0.
struct DelayedEdge 
{
    DelayedEdge() = default;
    DelayedEdge(int target, int weight, int t0, int p)
    : target(target), weight(weight), t0(t0), p(p)
    {

    }

    int target;
    int weight;
    int t0;
    int p;
};

using delayed_graph = std::vector<std::vector<DelayedEdge>>;


// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// with time tables.
/// Uses an adjacency list representation of a graph.
/// Nodes are numbered 0-(n-1)
struct TestCase {
    TestCase(int n, int m, int q, int s) 
    : n(n), m(m), q(q), s(s),
      adjacency(n), queries(q) {
        
    }

    int n;
    int m;
    int q;
    int s;
    std::vector<int> queries;
    delayed_graph adjacency;
    bool is_end() 
    {
        return n == 0 && m == 0 && q == 0 && s == 0;
    }
};


/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
TestCase process_input() 
{
    int n, m, q, s;
    cin >> n >> m >> q >> s;
    TestCase testCase(n, m, q, s);
    if (testCase.is_end()) {
        return testCase;
    }

    int u, v, t0, p, d;

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> t0 >> p >> d;

        testCase.adjacency[u].push_back(DelayedEdge(v, d, t0, p));
    }

    int query;
    for (int i = 0; i < q; i++) {
        cin >> query;
        testCase.queries[i] = query;
    }
    return testCase;
}

/// @brief Helper function to print the results. 
/// Prints Impossible if the magic number of max_int is encountered,
/// otherwise the actual distances.
void print_result(std::vector<ll>& results) 
{
    for (auto result : results) {
        if (result == std::numeric_limits<ll>::max()) {
            std::cout << "Impossible\n";
        }
        else {
            std::cout << result << "\n";    
        }
    }
}

/// @brief Calculates, given an arrival time, how long
/// the wait time is before an edge can be traversed.
/// @param arrival_time The time at which there is a need to traverse the edge.
/// @param t0 The earliest time the edge is available.
/// @param p The interval at which the edge is available.
/// @return How long the wait time is, or -1 if the edge is never available
/// after the arrival time.
ll calculate_delay(int arrival_time, int t0, int p) 
{
    ll edge_delay = 0;

    if (arrival_time <= t0) {
        edge_delay = t0 - arrival_time;
    }
    else if (p == 0) {
        edge_delay = -1;
    }
    else {
        edge_delay = (p - (arrival_time - t0) % p) % p;
    }
    return edge_delay;
}

/// @brief Calculates the actual time it takes to traverse an edge,
/// including wait time.
/// @param arrival_time The time at which there is a need to traverse the edge.
/// @param t0 The earliest time the edge is available.
/// @param p The interval at which the edge is available.
/// @param edge_weight The time it takes to traverse the edge without wait 
/// times included.
/// @return The actual time it takes to traverse an edge including wait time, or -1 
/// if the edge is never available after the arrival time.
ll calculate_edge_dist(int arrival_time, int t0, int p, int edge_weight) 
{
    ll edge_delay = calculate_delay(arrival_time, t0, p);
    if (edge_delay == -1) {
        return -1;
    }
    return edge_delay + edge_weight;
}


/// @brief Identifies the shortest paths from a source node 
/// to each node in a graph using Djikstra's algorithm and 
/// dynamically calculates the edge distances using a time table specification.
/// The function uses O((V+E)logV) time where V is the vertex count
/// and E is the edge count.
/// @param graph an adjacency list representation of a graph
/// @param start the node to identify paths from
/// @return A pair where the first element is the distances to each node identified
/// by their position in the list, and the second element is the immediate parent of each
/// node in the graph, when following the shortest path to that node.
std::pair<std::vector<ll>, std::map<int, int>> shortest_path_with_timetable(
    delayed_graph& graph, int start)
{
    const ll MAX_DIST = std::numeric_limits<ll>::max();
    int n = graph.size();
    std::vector<ll> distances(n, MAX_DIST);
    std::vector<int> visited(n, false);
    std::map<int, int> parents;

    auto comp = [](DistNode<ll> a, DistNode<ll> b ) { 
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
        auto node = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        visited[node.n] = true;
        if (distances[node.n] < node.dist) {
            continue;
        }
        for (auto edge: graph[node.n]) {
            int edge_dist = calculate_edge_dist(distances[node.n], edge.t0, edge.p, edge.weight);
            if (edge_dist == -1) {
                continue;
            } 
            
            ll next_distance = distances[node.n] + edge_dist;
            if (next_distance < distances[edge.target]) {
                distances[edge.target] = next_distance;
                unvisited.insert(DistNode<ll>(edge.target, distances[edge.target]));
                parents[edge.target] = node.n;
            }
        }
    }

    return std::make_pair(distances, parents);
}

/// @brief Wrapper method that handles identifying shortest paths and then querying
/// the result for specific target nodes.
/// @param graph an adjacency list representation of a graph
/// @param start the node to identify paths from
/// @param queries the nodes for which there is a need to identify
/// the travel times to.
/// @return a list of distances, one for each query.
std::vector<ll> solve(delayed_graph& graph, int start,
    std::vector<int>& queries) {

    auto paths = shortest_path_with_timetable(graph, start);
    std::vector<ll> dists;
    for (auto query: queries) {
        dists.push_back(paths.first[query]);
    }
    return dists;
}


/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true) {
        auto testCase = process_input();
        if (testCase.is_end()) {
            break;
        }
        auto dists = solve(testCase.adjacency, testCase.s, testCase.queries);
        print_result(dists);
    }

    std::cout.flush();
}