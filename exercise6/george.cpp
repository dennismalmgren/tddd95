//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>
std::ifstream cin("in_george.txt");
//auto& cin = std::cin;
using namespace algo;

struct TestCase {
    TestCase(int n, int m, int a, int b, int k, int g) 
    : n(n), m(m), a(a), b(b), k(k), g(g)
        , george_intersections(g), adjacency(n) {
        
    }

    int n;
    int m;
    int a;
    int b;
    int k;
    int g;

    std::vector<int> george_intersections;
    adjacency_graph adjacency;
};

TestCase process_input() 
{
    int n, m;
    cin >> n >> m;
    int a, b, k, g;
    cin >> a >> b >> k >> g;
    TestCase testCase(n, m, a - 1, b - 1, k, g);
    for (int i = 0; i < g; i++) {
        int george_intersection;
        cin >> george_intersection;
        testCase.george_intersections[i] = george_intersection - 1;
    }

    int u, v, w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        testCase.adjacency[u - 1].push_back(Edge(u - 1, v - 1, w));
        testCase.adjacency[v - 1].push_back(Edge(v - 1, u - 1, w));
    }
    return testCase;
}

void print_result(int result) 
{
    std::cout << result << "\n";
}

std::pair<std::vector<int>, std::map<int, int>> shortest_path_with_delay(
    adjacency_graph& graph, int start,
    std::vector<std::vector<int>>& george_arrival)
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    int n = graph.size();
    std::vector<int> distances(n, MAX_DIST);
    std::map<int, int> parents;

    auto comp = [](DistNode a, DistNode b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else {
            return a.n < b.n;
        }
    };

    //std::priority_queue<Node, std::vector<Node>, decltype(comp)> unvisited(comp);
    std::set<DistNode, decltype(comp)> unvisited(comp);

    DistNode root;
    root.dist = 0;
    root.n = start;
    unvisited.insert(root);
    distances[start] = 0;

    while (unvisited.size() > 0) {
        auto node = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        for (auto edge: graph[node.n]) {    
            // at the time of arrival to a source node going for a target node via an edge:
            // has George arrived? when did he arrive?
            // if node_arrival + edge_dist < george_arrival, then edge_delay = 0
            // if node_arrival > george_arrival + edge_dist, then edge_delay = 0
            // else edge_delay = george_arrival + edge_dist - node_arrival
            // basically we need georges arrival at each edge.
            int edge_delay = 0;
            if (distances[node.n] < george_arrival[node.n][edge.target]) {
                edge_delay = 0;
            }
            else if (distances[node.n] >= george_arrival[node.n][edge.target] + edge.weight) {
                edge_delay = 0;
            }
            else {
                auto path_travelled = distances[node.n] - george_arrival[node.n][edge.target];
                edge_delay = edge.weight - path_travelled;
            }
         

            if (distances[edge.target] > distances[node.n] + edge.weight + edge_delay) {
                if (distances[edge.target] != MAX_DIST) {
                    DistNode searchNode;
                    searchNode.dist = distances[edge.target];
                    searchNode.n = edge.target;
                    unvisited.erase(searchNode);
                }
                distances[edge.target] = distances[node.n] + edge.weight + edge_delay;
                DistNode unvisitedNode;
                unvisitedNode.dist = distances[edge.target];
                unvisitedNode.n = edge.target;
                unvisited.insert(unvisitedNode);
                parents[edge.target] = node.n;
            }
        }
    }

    return std::make_pair(distances, parents);
}

int solve(adjacency_graph& graph, int a, int b, int k, std::vector<int>& george_intersections) {
    int n = graph.size();
    std::vector<std::vector<int>> george_arrival(n, std::vector<int>(n, -2000));

    int current_arrival = -k;
    for (int i = 0; i < george_intersections.size() - 1; i++) {
        auto source = george_intersections[i];
        auto target = george_intersections[i + 1];
        george_arrival[source][target] = current_arrival;
        george_arrival[target][source] = current_arrival;
        for (auto& edge : graph[source]) {
            if (edge.target == target) {
                current_arrival += edge.weight;
                break;
            }
        }
   }

    auto paths = shortest_path_with_delay(graph, a, george_arrival);
    auto dist = paths.first[b];
    return dist;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    auto dist = solve(testCase.adjacency, testCase.a, testCase.b, testCase.k, testCase.george_intersections);
    print_result(dist);

    std::cout.flush();
}