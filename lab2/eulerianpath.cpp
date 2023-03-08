//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_eulerianpath.txt");
//auto& cin = std::cin;

using namespace algo;


struct TestCase {
    TestCase(int n, int m) 
    : n(n), m(m),
      adjacency(n) {
        
    }

    int n;
    int m;
    adjacency_graph adjacency;
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

    for (int i = 0; i < m; i++) {
        cin >> u >> v;

        testCase.adjacency[u].push_back(Edge(u, v, 1));
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


std::vector<int> solve(adjacency_graph& graph) {

    std::vector<int> path;
    return path;
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