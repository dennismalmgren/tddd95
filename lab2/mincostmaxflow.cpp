// mincostmaxflow.cpp
// Author: Dennis Malmgren
// Finds the maximum flow in a graph with a source
// and a sink, with the minimum cost.

#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_mincostmaxflow.txt");
//auto& cin = std::cin;

using ll = long long;

// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph, and the
/// same graph with reversed edges.
struct TestCase {
    TestCase(int n, int m, int s, int t) : n(n), m(m), s(s), t(t), 
                                            graph(n),
                                            reverse(n),
                                            flow(n, std::vector<ll>(n, 0)),
                                            cost(n, std::vector<ll>(n, 0)),
                                            capacity(n, std::vector<ll>(n, 0))
                                            {

    }

    int n;
    int m;
    int s;
    int t;
    adjacency_graph graph;
    adjacency_graph reverse;
    
    std::vector<std::vector<ll>> flow;
    std::vector<std::vector<ll>> cost;
    std::vector<std::vector<ll>> capacity;
};


/// @brief Reads the graph edges, capacity and cost from input.
/// @param testCase The pre-allocated vectors to fill with edges.
void process_testcase(TestCase& testCase) 
{
    int u, v, c, w;
    for (int i = 0; i < testCase.m; i++) {
        cin >> u >> v >> c >> w;
        testCase.graph[u].push_back(Edge(u, v, 0));
        testCase.reverse[v].push_back(Edge(v, u, 0));
        testCase.capacity[u][v] = c;
        testCase.cost[u][v] = w;
    }
}

/// @brief Helper function to print results of a min-cost-max-flow operation
/// @param flow The max flow achieved.
/// @param cost The minimum cost for that flow.
void print_result(ll flow, ll cost) 
{
    std::cout << flow << " " << cost << "\n";    
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
    std::pair<ll, ll> cost_flow = min_cost_max_flow(testCase.n, testCase.graph, 
    testCase.reverse, testCase.capacity, 
    testCase.flow, testCase.cost, testCase.s, testCase.t);
    print_result(cost_flow.second, cost_flow.first);
    std::cout.flush();
}