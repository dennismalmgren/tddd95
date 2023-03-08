//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_eulerianpath.txt");
//auto& cin = std::cin;

using namespace algo;


struct TestCase {
    TestCase(int n, int m) 
    : n(n), m(m),
      adjacency(n, std::vector<int>(n)),
      degrees(n) {
        
    }

    int n;
    int m;
    std::vector<std::vector<int>> adjacency;
    std::vector<int> degrees;
    bool is_end() 
    {
        return n == 0 && m == 0;
    }
};

TestCase process_input() 
{
    int n, m;
    cin >> n >> m;
    TestCase testCase(n, m);
    if (testCase.is_end()) {
        return testCase;
    }

    int u, v, w;
    int odd_count = n;
    for (int i = 0; i < m; i++) {
        cin >> u >> v;
        testCase.adjacency[u][v]++;
        testCase.adjacency[v][u]++;
        degrees[u]++;
        degrees[v]++;
    }

    return testCase;
}

void print_result(std::vector<int>& result) 
{
    if (result.size() == 0) {
        std::cout << "Impossible\n";
    }
    else {
        for (int i = 0; i < result.size(); i++) {
            std::cout << result[i];
            if (i != result.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

std::vector<int> solve(std::vector<std::vector<int>>& graph, std::vector<int>& degrees) {

    std::vector<int> result;
    int n = graph.size();
    std::vector<int> odd_vertices;
    int first = -1;
    for (int i = 0; i < n; i++) {
        if (degrees[i] % 2 == 1) {
            odd_vertices.push_back(i);
        }
        if (degrees[i] > 0 && first == -1) {
            first = i;
        }
    }

    if (odd_vertices.size() == 1 || odd_vertices.size() > 2) {
        return result; // Impossible
    }

    if (odd_vertices.size() == 2) {
        // Add an edge.
        graph[odd_vertices[0]][odd_vertices[1]]++;
        graph[odd_vertices[0]][odd_vertices[1]]++;
    }

    std::stack<int> path;
    path.push(first);
    while (!path.empty()) {
        int top = path.top();
        path.pop();
        if (degrees[top] == 0) {
            result.push_back(top);
        }
    }

    // all nodes need to have even degree except for 0 or 2.
    return result;
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
        auto result = solve(testCase.adjacency);
        print_result(result);
    }

    std::cout.flush();
}