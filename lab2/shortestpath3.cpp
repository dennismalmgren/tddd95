// shortestpath3.cpp
// Author: Dennis Malmgren
// Finds the single source shortest paths in a directed graph
// handles negative weights, but only works with
// integer distances.
#include "algo_lib.h"
#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_shortestpath3.txt");
//auto& cin = std::cin;

using namespace algo;

// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph.
/// Nodes are numbered 1-(n)
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
    adjacency_graph adjacency;
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

    int u, v, w;

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;

        testCase.adjacency[u].push_back(Edge(u, v, w));
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
/// prints -Infinity if the magic number of min_int is encountered,
/// otherwise the actual result.
void print_result(std::vector<int>& results) 
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    const int MIN_DIST = std::numeric_limits<int>::min();

    for (auto result : results) {
        if (result == MAX_DIST) {
            std::cout << "Impossible\n";
        }
        else if (result == MIN_DIST) {
            std::cout << "-Infinity\n";
        }
        else {
            std::cout << result << "\n";    
        }
    }
}

/// @brief Identifies the shortest paths in a graph using the Bellman-Ford method.
/// It can handle negative weights and negative cycles. Time complexity: O(VE).
/// @tparam T Type element in the graph.
/// @param graph Adjacency list representation of the graph.
/// @param start Start node to find paths from
template<typename T>
std::pair<std::vector<T>, std::map<int, int>> shortest_path_negative_weights(base_adjacency_graph<T>& graph, 
int start)
{
    const T MAX_DIST = std::numeric_limits<T>::max();
    const T MIN_DIST = std::numeric_limits<T>::min();
    
    int n = graph.size();
    std::vector<T> distances(n, MAX_DIST);
    distances[start] = 0;

    std::map<int, int> parents;

    for (int i = 0; i < n - 1; i++) {
        for (auto& node : graph) {
            for (auto& edge: node) {
                if (distances[edge.source] < MAX_DIST) {
                    if (distances[edge.source] + edge.weight < distances[edge.target]) {
                        distances[edge.target] = distances[edge.source] + edge.weight;
                        parents[edge.target] = edge.source;
                    }
                }
            }
        }
    }
    
    // Run it again..
    for (int i = 0; i < n - 1; i++) {
        for (auto& node : graph) {
            for (auto& edge: node) {
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

    return std::make_pair(distances, parents);
}

/// @brief Wrapper method that handles identifying shortest paths and then querying
/// the result for specific target nodes.
/// @param graph an adjacency list representation of a graph
/// @param start the node to identify paths from
/// @param queries the nodes for which there is a need to identify
/// the travel times to.
/// @return a list of distances, one for each query.
std::vector<int> solve(adjacency_graph& graph, int start,
    std::vector<int>& queries) {

    auto paths = shortest_path_negative_weights(graph, start);
    std::vector<int> dists;
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