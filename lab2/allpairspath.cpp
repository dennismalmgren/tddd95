//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_allpairspath.txt");
//auto& cin = std::cin;

using namespace algo;


struct TestCase {
    TestCase(int n, int m, int q) 
    : n(n), m(m), q(q),
      adjacency(n), 
      queries(q)
      {
        
    }

    int n;
    int m;
    int q;
    adjacency_graph adjacency;
    std::vector<std::pair<int, int>> queries;
    //std::vector<std::vector<int>> distances;

    bool is_end() 
    {
        return n == 0 && m == 0 && q == 0;
    }
};

TestCase process_input() 
{
    int n, m, q, s;
    cin >> n >> m >> q;
    TestCase testCase(n, m, q);
    if (testCase.is_end()) {
        return testCase;
    }

    int u, v, w;

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;

        testCase.adjacency[u].push_back(Edge(u, v, w));
    }

    int query_start, query_end;
    for (int i = 0; i < q; i++) {
        cin >> query_start >> query_end;
        testCase.queries[i] = std::make_pair(query_start, query_end);
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
    std::cout << "\n";
}

template<typename T>
std::vector<std::vector<T>> shortest_paths_negative_weights(base_adjacency_graph<T>& graph)
{
    const T MAX_DIST = std::numeric_limits<T>::max();
    const T MIN_DIST = std::numeric_limits<T>::min();
    
    int n = graph.size();

    // TODO: Maybe move this to test case read.
    std::vector<std::vector<T>> distances(n, std::vector<int>(n, MAX_DIST));
    for (int i = 0; i < n; i++) {
        distances[i][i] = 0;
    }

    for (auto& node : graph) {
        for (auto& edge : node) {
            distances[edge.source][edge.target] = std::min(distances[edge.source][edge.target], edge.weight);
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (distances[i][k] < MAX_DIST && distances[k][j] < MAX_DIST) {
                    distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
                }
            }
        }
    }

    // Now run it again.
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (distances[i][k] < MAX_DIST && distances[k][j] < MAX_DIST) {
                    if (distances[i][k] == MIN_DIST || distances[k][j] == MIN_DIST) {
                        distances[i][j] = MIN_DIST;
                    }
                    else if (distances[i][k] + distances[k][j] < distances[i][j]) {
                        distances[i][j] = MIN_DIST;
                    }
                    else if (distances[k][k] < 0) {
                        distances[i][j] = MIN_DIST;
                    }
                }
            }
        }
    }

    return distances;
}


std::vector<int> solve(adjacency_graph& graph,
    std::vector<std::pair<int, int>>& queries) {

    auto paths = shortest_paths_negative_weights(graph);
    std::vector<int> dists;
    for (auto query: queries) {
        dists.push_back(paths[query.first][query.second]);
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
        auto dists = solve(testCase.adjacency, testCase.queries);
        print_result(dists);
    }

    std::cout.flush();
}