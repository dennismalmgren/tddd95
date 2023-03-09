#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;
using ll = long long;
std::ifstream cin("in_mincostmaxflow.txt");
//auto& cin = std::cin;

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

void process_testcase(TestCase& testCase) 
{
    int u, v, c, w;
    for (int i = 0; i < testCase.m; i++) {
        cin >> u >> v >> c >> w;
        testCase.graph[u].push_back(Edge(u, v, c));
        testCase.reverse[v].push_back(Edge(v, u, 0));
        testCase.capacity[u][v] = c;
        testCase.cost[u][v] = w;
    }
}

void print_result(ll flow, ll cost) 
{
    std::cout << flow << " " << cost << "\n";    
}

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