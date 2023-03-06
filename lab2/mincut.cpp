//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_mincut.txt");
//auto& cin = std::cin;

using namespace algo;


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

void print_result(std::vector<int>& results) 
{
    std::cout << results.size() << "\n";
    for (int i = 0; i < results.size(); i++) {
        std::cout << results[i] << "\n";
    }
}


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