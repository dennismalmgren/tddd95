// allpairspath.cpp
// Author: Dennis Malmgren
// Finds the all pairs shortest paths in a directed graph.
// Handles negative and positive integer weights.
#include "algo_lib.h"
#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_allpairspath.txt");
//auto& cin = std::cin;

using namespace algo;

// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph.
struct TestCase {
    TestCase(int n, int m, int q) 
    : n(n), m(m), q(q),
      adjacency(n), 
      queries(q)
      {
        
    }

    int n;
    int m;
    int q;
    adjacency_graph adjacency;
    std::vector<std::pair<int, int>> queries;

    bool is_end() 
    {
        return n == 0 && m == 0 && q == 0;
    }
};


/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
TestCase process_input() 
{
    int n, m, q, s;
    cin >> n >> m >> q;
    TestCase testCase(n, m, q);
    if (testCase.is_end()) {
        return testCase;
    }

    int u, v, w;

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;

        testCase.adjacency[u].push_back(Edge(u, v, w));
    }

    int query_start, query_end;
    for (int i = 0; i < q; i++) {
        cin >> query_start >> query_end;
        testCase.queries[i] = std::make_pair(query_start, query_end);
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
    std::cout << "\n";
}

/// @brief Identifies the shortest paths in a graph using the Floyd-Warshall method.
/// It can handle negative weights and negative cycles. Time complexity: O(V^3).
/// @tparam T Type element in the graph.
/// @param graph Adjacency list representation of the graph.
template<typename T>
std::vector<std::vector<T>> shortest_paths_negative_weights(base_adjacency_graph<T>& graph)
{
    const T MAX_DIST = std::numeric_limits<T>::max();
    const T MIN_DIST = std::numeric_limits<T>::min();
    
    int n = graph.size();

    std::vector<std::vector<T>> distances(n, std::vector<int>(n, MAX_DIST));
    for (int i = 0; i < n; i++) {
        distances[i][i] = 0;
    }

    for (auto& node : graph) {
        for (auto& edge : node) {
            distances[edge.source][edge.target] = std::min(distances[edge.source][edge.target], edge.weight);
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (distances[i][k] < MAX_DIST && distances[k][j] < MAX_DIST) {
                    distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
                }
            }
        }
    }

    // Now run it again.
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (distances[i][k] < MAX_DIST && distances[k][j] < MAX_DIST) {
                    if (distances[i][k] == MIN_DIST || distances[k][j] == MIN_DIST) {
                        distances[i][j] = MIN_DIST;
                    }
                    else if (distances[i][k] + distances[k][j] < distances[i][j]) {
                        distances[i][j] = MIN_DIST;
                    }
                    else if (distances[k][k] < 0) {
                        distances[i][j] = MIN_DIST;
                    }
                }
            }
        }
    }

    return distances;
}

/// @brief Wrapper method that handles identifying shortest paths and then querying
/// the result for specific target nodes.
/// @param graph an adjacency list representation of a graph
/// @param queries the nodes pairs for which there is a need to identify
/// the travel times between.
/// @return a list of distances, one for each query.
std::vector<int> solve(adjacency_graph& graph,
    std::vector<std::pair<int, int>>& queries) {

    auto paths = shortest_paths_negative_weights(graph);
    std::vector<int> dists;
    for (auto query: queries) {
        dists.push_back(paths[query.first][query.second]);
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
        auto dists = solve(testCase.adjacency, testCase.queries);
        print_result(dists);
    }

    std::cout.flush();
}