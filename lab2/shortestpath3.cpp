//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_shortestpath3.txt");
//auto& cin = std::cin;

using namespace algo;


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


std::vector<int> solve(adjacency_graph& graph, int start,
    std::vector<int>& queries) {
    std::vector<Edge> edges;
    for (auto& node : graph) {
        for (auto& edge : node) {
            edges.push_back(edge);
        }
    }
    std::vector<int> distances(graph.size(), std::numeric_limits<int>::max());
    std::vector<int> parents(graph.size(), -1);
    auto weight_fun = [](Edge& e) { return e.weight; };
    shortest_path_negative_weights(graph.size(), edges, distances, parents, start, weight_fun);
    std::vector<int> dists;
    for (auto query: queries) {
        dists.push_back(distances[query]);
    }
    return dists;
}

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