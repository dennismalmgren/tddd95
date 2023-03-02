#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

std::ifstream cin("in_island_hopping.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase(int m) :m(m), points(m) {
        
    }
    int m;

    std::vector<std::pair<double, double>> points;
};

int process_header()
{
    int n;
    cin >> n;
    return n;
}

TestCase process_input() 
{
    int m;
    cin >> m;
    double a, b;
    TestCase testCase(m);
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        testCase.points[i] = std::make_pair(a, b);
    }

   return testCase;
}

void print_result(double result) 
{
    std::cout << std::setprecision (17) <<result << "\n";
}


double solve(std::vector<std::pair<double, double>>& points) {
    double_adjacency_graph graph(points.size());
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            double weight = std::sqrt(std::pow(points[i].first - points[j].first, 2) + std::pow(points[i].second - points[j].second, 2));
            graph[i].push_back(DoubleEdge(i, j, weight));
            graph[j].push_back(DoubleEdge(j, i, weight));
        }
    }

    auto result = mst(graph);
    if (result.has_value()) {
        return result.value().first;
    } else {
        // shouldn't happen
        return -10000;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_cases = process_header();
    for (int i = 0; i < test_cases; i++) {
        auto testCase = process_input();
        auto result = solve(testCase.points);
        print_result(result);
    }
    std::cout.flush();
}