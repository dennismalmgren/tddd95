//distance/parent[] shortest_path(graph G, node start)

#include <bits/stdc++.h>
std::ifstream cin("in_shortestpath1.txt");
//auto& cin = std::cin;

struct Edge {
    public:
        int weight;
        int target;
};

struct TestCase {
    TestCase(int n, int m, int q, int s) : n(n), m(m), q(q), s(s), adjacency(n), queries(q) {
        
    }

    int n;
    int m;
    int q;
    int s;
    std::vector<int> queries;
    std::vector<std::vector<Edge>> adjacency;
};

TestCase process_input() 
{
    int n, m, q, s;
    cin >> n >> m >> q >> s;
    TestCase testCase(n, m, q, s);
    if (n == 0 && m == 0 && q == 0 && s == 0) {
        return testCase;
    }

    int u, v, w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        Edge edge;
        edge.target = v;
        edge.weight = w;
        testCase.adjacency[u].push_back(edge);    
    }

    int query;
    for (int i = 0; i < q; i++) {
        cin >> query;
        testCase.queries[i] = query;
    }
    return testCase;
}

void print_result(int result) 
{
    if (result == std::numeric_limits<int>::max()) {
        std::cout << "Impossible\n";
    }
    else {
        std::cout << result << "\n";
    }
}

struct Node 
{
    public:
        int n;
        int dist;
};

std::pair<std::vector<int>, std::map<int, int>> shortest_path(std::vector<std::vector<Edge>> graph, int start)
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    std::vector<int> distances(graph.size(), MAX_DIST);
    std::map<int, int> parents;

    auto comp = [](Node a, Node b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else {
            return a.n < b.n;
        }
    };

    //std::priority_queue<Node, std::vector<Node>, decltype(comp)> unvisited(comp);
    std::set<Node, decltype(comp)> unvisited(comp);

    Node root;
    root.dist = 0;
    root.n = start;
    unvisited.insert(root);
    distances[start] = 0;

    while (unvisited.size() > 0) {
        auto node = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        for (auto edge: graph[node.n]) {    
            if (distances[edge.target] > distances[node.n] + edge.weight) {
                if (distances[edge.target] != MAX_DIST) {
                    Node searchNode;
                    searchNode.dist = distances[edge.target];
                    searchNode.n = edge.target;
                    unvisited.erase(searchNode);
                }
                distances[edge.target] = distances[node.n] + edge.weight;
                Node unvisitedNode;
                unvisitedNode.dist = distances[edge.target];
                unvisitedNode.n = edge.target;
                unvisited.insert(unvisitedNode);
                parents[edge.target] = node.n;
            }
        }
    }

    return std::make_pair(distances, parents);
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true) {
        auto testCase = process_input();
        if (testCase.m == 0 && testCase.n == 0 && testCase.q == 0 && testCase.s == 0) {
            break;
        }
        auto paths = shortest_path(testCase.adjacency, testCase.s);
        for (auto query: testCase.queries) {
            auto dist = paths.first[query];
            print_result(dist);
        }
    }

    std::cout.flush();
}