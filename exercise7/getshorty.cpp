#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

std::ifstream cin("in_getshorty.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase(int n, int m) : n(n), m(m), corridors(n), factors(n, std::vector<double>(n, 0)) {
        
    }

    int n;
    int m;

    double_adjacency_graph corridors;
    std::vector<std::vector<double>> factors;

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
    if (n == 0 && m == 0) {
        return testCase;
    }

    int u, v;
    double w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        testCase.corridors[u].push_back(DoubleEdge(u, v, -std::log(w)));
        testCase.corridors[v].push_back(DoubleEdge(v, u, -std::log(w)));
        testCase.factors[u][v] = w;    
        testCase.factors[v][u] = w;    
    }
    return testCase;
}

void print_result(double result) 
{
    std::cout << std::fixed <<
    std::setprecision(4) << result << "\n";
}

double extract_dist(std::map<int, int>& parents, std::vector<std::vector<double>>& factors)
{
    double result = 1.0;
    int current = factors.size() - 1;
    while (current != 0) {
        int parent = parents[current];
        result *= factors[parent][current];
        current = parent;
    }
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
        auto paths = shortest_path(testCase.corridors, 0);
        auto dist = extract_dist(paths.second, testCase.factors);
        print_result(dist);
    }

    std::cout.flush();
}