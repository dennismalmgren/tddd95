#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

std::ifstream cin("in_risk.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase(int n) : n(n), armies(n), graph(n*2 + 2), border_region(n, false) {
        
    }

    int n;
    std::vector<int> armies;
    adjacency_graph graph;
    std::vector<bool> border_region;
};

int process_header() 
{
    int n;
    cin >> n;
    return n;
}

TestCase process_input() 
{
    int n, a;
    cin >> n;

    TestCase testCase(n);

    for (int i = 0; i < n; i++) {
        cin >> a;
        testCase.armies[i] = a;
    }
    
    std::string neighbors;
    for (int i = 0; i < n; i++) {
        cin >> neighbors;
        if (testCase.armies[i] == 0) { // Not our region.
            continue;
        }
        testCase.graph[i].push_back(Edge(i, i + n, 0));
        testCase.graph[i + n].push_back(Edge(i + n, i, 0)); // backward edge.

        for (int j = 0; j < n; j++) {
            if (neighbors[j] == 'Y') {
                if (testCase.armies[j] == 0) {
                    testCase.border_region[i] = true;
                    // Add edges to sink.
                }
                else { // We share a border, but with a region of our own.
                    testCase.graph[i].push_back(Edge(i, j + n, 0));
                    testCase.graph[j + n].push_back(Edge(j + n, i, 0));
                    //testCase.graph[j].push_back(Edge(j, i, 0));
                }
            }
        }    
                // add edges to source.
        testCase.graph[n*2].push_back(Edge(n*2, i, 0));
        testCase.graph[i].push_back(Edge(i, n*2, 0));
        // add edges to sink
        testCase.graph[i + n].push_back(Edge(i + n, n*2 + 1, 0));
        testCase.graph[n*2 + 1].push_back(Edge(n*2 + 1, i + n, 0));
    }
    return testCase;
}

void print_result(int result) 
{
    std::cout << result << "\n";

}

int solve(adjacency_graph& graph, std::vector<int>& armies, std::vector<bool>& is_border_region)
{
    int min_capacity = std::numeric_limits<int>::max();
    int total_capacity = 0;
    int controlled_regions = 0;
    int controlled_border_regions = 0;
    int controlled_non_border_regions = 0;
    int num_regions = armies.size();
    // At worst, we stay the same.
    // At best, the total 
    for (int i = 0; i < armies.size(); i++) {
        total_capacity += armies[i];
        if (armies[i] > 0) {
            controlled_regions++;
            if (is_border_region[i]) {
                controlled_border_regions++;
                min_capacity = std::min(min_capacity, armies[i]);
            }
            else {
                controlled_non_border_regions++;
            }
        }
    }
    
    // At worst, we stay the same, i.e. current min capacity in border regions.
    // At best, there is one unit in each region, and the rest are evenly distributed.
    int max_capacity = (total_capacity - controlled_non_border_regions) / controlled_border_regions + 1;
    while (max_capacity - min_capacity > 1) {
        int test_capacity = (max_capacity + min_capacity) / 2;
        // Create capacities.
        std::vector<std::vector<int>> capacity(graph.size(), std::vector<int>(graph.size(), 0));
        for (int i = 0; i < num_regions; i++) {
            bool is_own_region = armies[i] > 0;
            if (!is_own_region) {
                continue;
            }
            for (auto& edge : graph[i]) {
                if (edge.target != graph.size() - 2) {
                    capacity[i][edge.target] = armies[i];
                }
            }
            if (armies[i] > 0) {
                capacity[graph.size() - 2][i] = armies[i];
            }
            capacity[i][i + num_regions] = armies[i];
            if (is_border_region[i]) {
                    capacity[i + num_regions][graph.size() - 1] = test_capacity;
            }
            else {
                capacity[i + num_regions][graph.size() - 1] = 1;
            }
        }

        // Create flow.
        std::vector<std::vector<int>> flow(graph.size(), std::vector<int>(graph.size(), 0));
        auto the_flow = max_flow(graph, capacity, flow, graph.size() - 2, graph.size() - 1);
        bool flow_achieved = true;
        for (int i = 0; i < num_regions; i++) {
            if (armies[i] > 0 && is_border_region[i]) {
                if (flow[i + num_regions][graph.size() - 1] != test_capacity) {
                    flow_achieved = false;
                    break;
                }
            }
            else if (armies[i] > 0) {
                if (flow[i + num_regions][graph.size() - 1] != 1) {
                    flow_achieved = false;
                    break;
                }
            }
        }
        if (flow_achieved) {
            min_capacity = test_capacity;
        }
        else {
            max_capacity = test_capacity;
        }
    }

    return min_capacity;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int num_testcases = process_header();
    for (int i = 0; i < num_testcases; i++) {
        auto testcase = process_input();
        auto result = solve(testcase.graph, testcase.armies, testcase.border_region);
        print_result(result);
    }

    std::cout.flush();
}