
// shortestpath1.cpp
// Author: Dennis Malmgren
// Finds the single source shortest paths in a directed graph
// does not handle negative weights, and only works with
// integer distances.

#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_shortestpath1.txt");
//auto& cin = std::cin;

/// @brief Class that contains information about edges
/// in a graph. Keeps track of the cost (weight) to transition 
/// the edge as well as the target node it connects to.
struct Edge {
    public:
        int weight;
        int target;
};

/// @brief Class that contains information about nodes
/// in a graph. Keeps track of distance estimates to the node.
struct Node 
{
    public:
        int n;
        int dist;
};

// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph.
/// Nodes are numbered 0-(n-1)
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


/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
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


/// @brief Helper function to print the results. 
/// Prints Impossible if the magic number of max_int is encountered,
/// otherwise the actual result.
void print_result(int result) 
{
    if (result == std::numeric_limits<int>::max()) {
        std::cout << "Impossible\n";
    }
    else {
        std::cout << result << "\n";
    }
}


/// @brief Identifies the shortest paths from a source node 
/// to each node in a graph using Djikstra's algorithm. 
/// The function uses O((V+E)logV) time where V is the vertex count
/// and E is the edge count.
/// @param graph an adjacency list representation of a graph
/// @param start the node to identify paths from
/// @return A pair where the first element is the distances to each node identified
/// by their position in the list, and the second element is the immediate parent of each
/// node in the graph, when following the shortest path to that node.
std::pair<std::vector<int>, std::map<int, int>> 
shortest_path(std::vector<std::vector<Edge>> graph, int start)
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    std::vector<int> distances(graph.size(), MAX_DIST);
    std::map<int, int> parents;

    auto comp = [](const Node& a, const Node& b ) { 
        if (a.dist != b.dist) {
            return a.dist < b.dist; 
        }
        else {
            return a.n < b.n;
        }
    };

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

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
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