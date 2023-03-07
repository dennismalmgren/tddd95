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

template<typename T>
std::pair<std::vector<T>, std::map<int, int>> shortest_path_negative_weights(base_adjacency_graph<T>& graph, 
int start)
{
    const T MAX_DIST = std::numeric_limits<T>::max();
    const T MIN_DIST = std::numeric_limits<T>::min();
    
    int n = graph.size();
    std::vector<T> distances(n, MAX_DIST);
    distances[start] = 0;

    std::map<int, int> parents;

    for (int i = 0; i < n - 1; i++) {
        for (auto& node : graph) {
            for (auto& edge: node) {
                if (distances[edge.source] < MAX_DIST) {
                    if (distances[edge.source] + edge.weight < distances[edge.target]) {
                        distances[edge.target] = distances[edge.source] + edge.weight;
                        parents[edge.target] = edge.source;
                    }
                }
            }
        }
    }
    
    //std::vector<T> distances_copy = distances;
    
    // Run it again..
    for (int i = 0; i < n - 1; i++) {
        for (auto& node : graph) {
            for (auto& edge: node) {
                if (distances[edge.source] < MAX_DIST) {
                    if (distances[edge.source] == MIN_DIST) {
                        distances[edge.target] = MIN_DIST;
                        parents[edge.target] = edge.source;
                    }
                    else if (distances[edge.source] + edge.weight < distances[edge.target]) {
                        distances[edge.target] = MIN_DIST;
                        parents[edge.target] = edge.source;
                    }
                }
            }
        }
    }

    return std::make_pair(distances, parents);
}


std::vector<int> solve(adjacency_graph& graph, int start,
    std::vector<int>& queries) {

    auto paths = shortest_path_negative_weights(graph, start);
    std::vector<int> dists;
    for (auto query: queries) {
        dists.push_back(paths.first[query]);
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