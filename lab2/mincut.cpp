// mincut.cpp
// Author: Dennis Malmgren
// Finds the minimum cut in a graph, i.e., the
// minimal weight edges that need to be removed to separate
// source from sink.
#include "algo_lib.h"
#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_mincut.txt");
//auto& cin = std::cin;

using namespace algo;

// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph.
struct TestCase {
    TestCase(int n, int m, int s, int t) 
    : n(n), m(m), s(s), t(t),
      adjacency(n),
      flow(n, std::vector<int>(n, 0)),
      capacity(n, std::vector<int>(n, 0)) {
        
    }

    int n;
    int m;
    int s;
    int t;
    adjacency_graph adjacency;
    std::vector<std::vector<int>> flow;
    std::vector<std::vector<int>> capacity;

};


/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
TestCase process_input() 
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    TestCase testCase(n, m, s, t);

    int u, v, w;

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;

        testCase.adjacency[u].push_back(Edge(u, v, w));
        testCase.adjacency[v].push_back(Edge(v, u, 0));
        testCase.capacity[u][v] = w;
    }

    return testCase;
}

/// @brief Helper function to print the result. Prints the number of vertices
/// and their numbers.
/// @param results the vertices that together with the source node
/// are separated from the sink.
void print_result(std::vector<int>& results) 
{
    std::cout << results.size() << "\n";
    for (int i = 0; i < results.size(); i++) {
        std::cout << results[i] << "\n";
    }
}

/// @brief Minimum cut algorithm. Uses the max flow algorithm and 
/// then breadth-first serach in order to identify the reachable 
/// nodes when flow has been saturated. 
/// Time complexity: O(EF) where F is the maximum flow of the network.
/// @param graph The graph to investigate.
/// @param capacity Capacity between node-node pairs.
/// @param flow Current flow between node-node pairs.
/// @param start Source node.
/// @param end Sink node.
/// @return A list of vertices that together with the source node
/// are separated from the sink node by a minimum cut.
std::vector<int> mincut(adjacency_graph& graph, 
                        std::vector<std::vector<int>>& capacity,
                        std::vector<std::vector<int>>& flow,
                        int start, int end) {
    
    int the_flow = max_flow(graph, capacity, flow, start, end);
    std::vector<bool> visited(graph.size(), false);
    std::vector<int> parents(graph.size(), -2);

    bfs_flow(graph, parents, capacity, visited, start, end);
    std::vector<int> results;
    for (int i = 0; i < graph.size(); i++) {
        if (visited[i]) {
            results.push_back(i);
        }
    }

    return results;
}

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    auto vertices = mincut(testCase.adjacency, 
                            testCase.capacity, 
                            testCase.flow, 
                            testCase.s, 
                            testCase.t);
    print_result(vertices);

    std::cout.flush();
}