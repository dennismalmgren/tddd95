//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_mincut.txt");
//auto& cin = std::cin;

using namespace algo;


struct TestCase {
    TestCase(int n, int m, int s, int t) 
    : n(n), m(m), s(s), t(t),
      adjacency(n) {
        
    }

    int n;
    int m;
    int s;
    int t;
    adjacency_graph adjacency;

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

std::vector<int> mincut(adjacency_graph& graph, int start, int end) {
    std::vector<int> results;
    return results;
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



int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    auto vertices = mincut(testCase.adjacency, testCase.s, testCase.t);
    print_result(vertices);

    std::cout.flush();
}