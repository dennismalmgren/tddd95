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
        if (result == std::numeric_limits<int>::max()) {
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

void build_adjacency_graph_roads(adjacency_graph& graph, std::vector<Edge>& roads) 
{
    for (auto& edge: roads) {
        graph[edge.source].push_back(edge);
        graph[edge.target].push_back(Edge(edge.target, edge.source, edge.weight));
    }
}

struct FuelNode
{
    FuelNode() = default;
    int city;
    int fuel;
    int dist;
};

int 
capped_shortest_path(adjacency_graph& graph, 
                    std::vector<int>& fuel_prices, 
                    int start, 
                    int end, 
                    int capacity)
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    int n = graph.size();
    int graph_size = n * (capacity + 1);
    
    auto comp2 = [](FuelNode a, FuelNode b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else if (a.fuel != b.fuel) {
            return a.fuel < b.fuel;
        } 
        else { 
            return a.city < b.city;
        }
    };

    auto comp = [](FuelNode a, FuelNode b ) { 
        if (a.dist != b.dist) {
            return a.dist > b.dist; 
        }
        else if (a.fuel != b.fuel) {
            return a.fuel > b.fuel;
        } 
        else { 
            return a.city > b.city;
        }
    };

    std::vector<std::vector<int>> distances(graph_size, std::vector<int>(101, MAX_DIST));
    std::vector<std::vector<bool>> visited(graph_size, std::vector<bool>(101, false));

    std::priority_queue<FuelNode, std::vector<FuelNode>, decltype(comp)> queue(comp);

    distances[start][0] = 0;
    queue.push(FuelNode{start, 0, 0});

    while (!queue.empty()) {
        auto node = queue.top();
        queue.pop();
        if (visited[node.city][node.fuel]) {
            continue;
        }
        if (node.city == end) {
            break;
        }

        // Enumerate edges. Gassing up.
        if (node.fuel < capacity &&
            node.dist + fuel_prices[node.city] < distances[node.city][node.fuel + 1]) {
            distances[node.city][node.fuel + 1] = node.dist + fuel_prices[node.city];
            queue.push(FuelNode{node.city, node.fuel + 1, distances[node.city][node.fuel + 1]});
        }

        // Enumerate edges. Traveling.
        for (int i = 0; i < graph[node.city].size(); i++) {
            auto edge = graph[node.city][i];
            if (node.fuel >= edge.weight &&
                    distances[node.city][node.fuel] < distances[edge.target][node.fuel - edge.weight]) {
                    distances[edge.target][node.fuel - edge.weight] = distances[node.city][node.fuel];
                    queue.push(FuelNode{edge.target, node.fuel - edge.weight, distances[edge.target][node.fuel - edge.weight]});
            }
        }
        visited[node.city][node.fuel] = true;
    }

    return distances[end][0];
}

std::vector<int> solve(std::vector<Edge>& roads,
                        std::vector<int>& fuel_prices, 
                        std::vector<int>& capacities,
                        std::vector<int>& starts,
                        std::vector<int>& ends) {
    std::vector<int> results(capacities.size());
    
    int capacity_dim = 101;

    adjacency_graph adjacency_graph(fuel_prices.size());
    int capacity_prev = 0;
                                
    build_adjacency_graph_roads(adjacency_graph, roads);

    for (int i = 0; i < capacities.size(); i++) {
        auto capacity = capacities[i];
        auto start = starts[i];
        auto end = ends[i];

        auto dist = capped_shortest_path(adjacency_graph, fuel_prices, start, end, capacity);
        results[i] = dist;
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