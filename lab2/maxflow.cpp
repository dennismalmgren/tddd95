// maxflow.cpp
// Author: Dennis Malmgren
// Finds the maximum flow in a graph with a source
// and a sink.

#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_maxflow.txt");
//auto& cin = std::cin;


// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph.
struct TestCase {
    TestCase(int n, int m, int s, int t) : n(n), m(m), s(s), t(t), g(n), 
                                            capacity(n, std::vector<int>(n, 0)), 
                                            flow(n, std::vector<int>(n, 0)) {
    }

    int n;
    int m;
    int s;
    int t;
    adjacency_graph g;
    std::vector<std::vector<int>> flow;
    std::vector<std::vector<int>> capacity;
};

/// @brief Reads the graph edges and capacity from input.
/// @param testCase The pre-allocated vectors to fill with edges.
void process_testcase(TestCase& testCase) 
{
    int u, v, c;
    for (int i = 0; i < testCase.m; i++) {
        cin >> u >> v >> c;
        Edge forward_edge(u, v, c);
        Edge backward_edge(v, u, 0);
        testCase.g[u].push_back(forward_edge);
        testCase.g[v].push_back(backward_edge);
        testCase.capacity[u][v] = c;
    }
}

/// @brief Helper function to print the results.
/// @param the_graph The graph that was investigated
/// @param t The target node
/// @param flow The total flow
/// @param the_flow The flow between each node-node pair.
void print_result(adjacency_graph& the_graph, int t, int flow, std::vector<std::vector<int>>& the_flow) 
{
    // Determine max flow.
    // Determine edges used.
    int num_nodes = the_graph.size();
    std::vector<std::pair<int, int>> edges_used;
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (the_flow[i][j] > 0) {
                edges_used.push_back(std::make_pair(i, j));
            }
        }
    }
   
    std::cout << the_graph.size() << " " << flow << " " << edges_used.size() << "\n";
    for (int i = 0; i < edges_used.size(); i++) {
        auto& edge = edges_used[i];
        std::cout << edge.first << " " << edge.second << " " << the_flow[edge.first][edge.second] << "\n";
    }
}


/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    TestCase testCase(n, m, s, t);

    process_testcase(testCase);
    int flow = max_flow(testCase.g, testCase.capacity, testCase.flow, testCase.s, testCase.t);
    print_result(testCase.g, testCase.t, flow, testCase.flow);
    std::cout.flush();
}