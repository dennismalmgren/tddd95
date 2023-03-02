//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_fulltank.txt");
//auto& cin = std::cin;

using namespace algo;

struct TestCase {
    TestCase(int n, int m) 
    : n(n), m(m) {
        roads.reserve(m);
        fuel_price.reserve(n);
    }

    int n;
    int m;

    std::vector<int> fuel_price;
    std::vector<Edge> roads;
    std::vector<int> starts;
    std::vector<int> ends;
    std::vector<int> capacities;
};

void process_input(TestCase& testCase) 
{

    int p;
    int u, v, w;
    for (int i = 0; i < testCase.n; i++) {
        cin >> p;
        testCase.fuel_price.push_back(p);
    }

    for (int i = 0; i < testCase.m; i++) {
        cin >> u >> v >> w;
        testCase.roads.push_back(Edge(u, v, w));
    }

    int q;
    cin >> q;
    testCase.starts.reserve(q);
    testCase.ends.reserve(q);
    testCase.capacities.reserve(q);
    int start, end, capacity;
    for (int i = 0; i < q; i++) {
        cin >> capacity >> start >> end;
        testCase.starts.push_back(start);
        testCase.ends.push_back(end);
        testCase.capacities.push_back(capacity);
    }
}

void print_result(std::vector<int>& results) 
{
    for (auto result: results) {
        if (result < 0) {
            std::cout << "impossible\n";
        }
        else {
            std::cout << result << "\n";
        }
    }
}

void build_adjacency_graph(std::vector<Edge>& roads,
                                            std::vector<int>& fuel_prices,
                                            adjacency_graph& adjacency_graph,
                                            int capacity_prev,
                                        int capacity,
                                        int capacity_dim) 
{
    if (capacity_prev == capacity) {
        return;
    }
    // Add travel edges
    for (auto& edge: roads) {
        auto weight = edge.weight;
        for (auto source_capacity = std::max(weight, capacity_prev + 1); source_capacity <= capacity; source_capacity++) {
            auto source = edge.source * capacity_dim + source_capacity;
            auto target = edge.target * capacity_dim + source_capacity - weight;
            adjacency_graph[source].push_back(Edge(source, target, 0));
            source = edge.target * capacity_dim + source_capacity;
            target = edge.source * capacity_dim + source_capacity - weight;
            adjacency_graph[source].push_back(Edge(source, target, 0));
        }
    }
    for (int i = 0; i < fuel_prices.size(); i++) {
        for (int j = capacity_prev; j < capacity; j++) {
            auto source = i * capacity_dim + j;
            auto target = i * capacity_dim + j + 1;
            adjacency_graph[source].push_back(Edge(source, target, fuel_prices[i]));
        }
    }
}

std::vector<int> solve(std::vector<Edge>& roads,
                        std::vector<int>& fuel_prices, 
                        std::vector<int>& capacities,
                        std::vector<int>& starts,
                        std::vector<int>& ends) {
    std::vector<int> results(capacities.size());
    
    int capacity_dim = 101;

    adjacency_graph adjacency_graph(fuel_prices.size() * capacity_dim);
    int capacity_prev = 0;
    auto capacity_indices = argsort(capacities);

    for (int i = 0; i < capacities.size(); i++) {
        int capacity = capacities[capacity_indices[i]];

        auto start = starts[i];
        auto end = ends[i];

        build_adjacency_graph(roads, fuel_prices, adjacency_graph,
                                    capacity_prev, 
                                    capacity, 
                                    capacity_dim);

        auto paths = shortest_path(adjacency_graph, start * capacity_dim);

        int min_dist = MAX_DIST;
        int node_id = end * capacity_dim;
        std::vector<int>& distances = paths.first;
        for (int end_capacity = 0; end_capacity < capacity; end_capacity++) {
            min_dist = std::min(distances[node_id++], min_dist);
        }
        if (min_dist < MAX_DIST) {
            results[capacity_indices[i]] = min_dist;
        }
        else {
            results[capacity_indices[i]] = -1;
        }
        capacity_prev = capacity;
    }
    return results;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    TestCase testCase(n, m);
    process_input(testCase);
    auto dist = solve(testCase.roads,
                        testCase.fuel_price, 
                        testCase.capacities,
                        testCase.starts,
                        testCase.ends);
    print_result(dist);

    std::cout.flush();
}