//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>
std::ifstream cin("in_shortestpath2.txt");
//auto& cin = std::cin;
using namespace algo;
using ll = long long;

struct DelayedEdge 
{
    DelayedEdge() = default;
    DelayedEdge(int target, int weight, int t0, int p)
    : target(target), weight(weight), t0(t0), p(p)
    {

    }

    int target;
    int weight;
    int t0;
    int p;
};

using delayed_graph = std::vector<std::vector<DelayedEdge>>;

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
    delayed_graph adjacency;
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

    int u, v, t0, p, d;

    for (int i = 0; i < m; i++) {
        cin >> u >> v >> t0 >> p >> d;

        testCase.adjacency[u].push_back(DelayedEdge(v, d, t0, p));
    }

    int query;
    for (int i = 0; i < q; i++) {
        cin >> query;
        testCase.queries[i] = query;
    }
    return testCase;
}

void print_result(std::vector<ll>& results) 
{
    for (auto result : results) {
        if (result == std::numeric_limits<ll>::max()) {
            std::cout << "Impossible\n";
        }
        else {
            std::cout << result << "\n";    
        }
    }
}

ll calculate_delay(int arrival_time, int t0, int p) 
{
    ll edge_delay = 0;

    if (arrival_time <= t0) {
        edge_delay = t0 - arrival_time;
    }
    else if (p == 0) {
        edge_delay = -1;
    }
    else {
        edge_delay = (p - (arrival_time - t0) % p) % p;
    }
    return edge_delay;
}

ll calculate_edge_dist(int arrival_time, int t0, int p, int edge_weight) 
{
    ll edge_delay = calculate_delay(arrival_time, t0, p);
    if (edge_delay == -1) {
        return -1;
    }
    return edge_delay + edge_weight;
}

std::pair<std::vector<ll>, std::map<int, int>> shortest_path_with_timetable(
    delayed_graph& graph, int start)
{
    const ll MAX_DIST = std::numeric_limits<ll>::max();
    int n = graph.size();
    std::vector<ll> distances(n, MAX_DIST);
    std::vector<int> visited(n, false);
    std::map<int, int> parents;

    auto comp = [](DistNode<ll> a, DistNode<ll> b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else {
            return a.n < b.n;
        }
    };

    //std::priority_queue<DistNode<ll>, std::vector<DistNode<ll>>, decltype(comp)> unvisited(comp);
    std::set<DistNode<ll>, decltype(comp)> unvisited(comp);

    unvisited.insert(DistNode<ll>(start, 0));
    distances[start] = 0;

    while (unvisited.size() > 0) {
        auto node = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        visited[node.n] = true;
        if (distances[node.n] < node.dist) {
            continue;
        }
        for (auto edge: graph[node.n]) {
            int edge_dist = calculate_edge_dist(distances[node.n], edge.t0, edge.p, edge.weight);
            if (edge_dist == -1) {
                continue;
            } 
            
            ll next_distance = distances[node.n] + edge_dist;
            if (next_distance < distances[edge.target]) {
                //unvisited.erase(DistNode<ll>(edge.target, distances[edge.target]));

                distances[edge.target] = next_distance;
                unvisited.insert(DistNode<ll>(edge.target, distances[edge.target]));
                parents[edge.target] = node.n;
            }
        }
    }

    return std::make_pair(distances, parents);
}

std::vector<ll> solve(delayed_graph& graph, int start,
    std::vector<int>& queries) {

    auto paths = shortest_path_with_timetable(graph, start);
    std::vector<ll> dists;
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