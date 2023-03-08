//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_xyzzy.txt");
//auto& cin = std::cin;

using namespace algo;


struct TestCase {
    TestCase(int n) 
    : n(n) {
        if (n > 0) {
            room_costs.reserve(n + 1);
        }
    }

    int n;
    std::vector<Edge> edges;
    std::vector<int> room_costs;
    bool is_end() 
    {
        return n == 0;
    }
};

TestCase process_input() 
{
    int n;
    cin >> n;
    TestCase testCase(n + 1);
    if (testCase.is_end()) {
        return testCase;
    }
    testCase.room_costs.push_back(0);
    int e, d, r;
    for (int i = 0; i < n; i++) {
        cin >> e >> d;
        testCase.room_costs.push_back(e);
        for (int j = 0; j < d; j++) {
            cin >> r;
            testCase.edges.push_back(Edge(i + 1, r, 0));
        }
        testCase.room_costs[i + 1] = -e;
    }
    for (int i = 0; i < testCase.edges.size(); i++) {
        testCase.edges[i].weight = testCase.room_costs[testCase.edges[i].target];
    }
    testCase.edges.push_back(Edge(0, 1, -100));

    return testCase;
}

void print_result(bool winnable) 
{
    if (winnable) {
        std::cout << "winnable\n";
    }
    else {
        std::cout << "hopeless\n";
    }
}

bool solve(std::vector<Edge>& edges, std::vector<int>& room_costs) 
{
    // node id is room_number (0->n) times 2001 (max cost) + cost.
    // room costs is n+1 size.
    int n = room_costs.size();
    std::vector<int> distances(n, std::numeric_limits<int>::max());
    std::vector<int> parents(n, -1);
    int start = 0;
    shortest_path_negative_weights(n, edges, distances, parents, start);
    return distances[n - 1] < 0;
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
        bool winnable = solve(testCase.edges, testCase.room_costs);
        print_result(winnable);
    }

    std::cout.flush();
}