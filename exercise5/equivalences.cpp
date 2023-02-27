#include <bits/stdc++.h>
std::ifstream cin("in_equivalences.txt");
//auto& cin = std::cin;


struct Node {
    int n;
    int dist;
};

struct Edge {
    public:
        Edge(int source, int target, int weight) 
        : source(source), target(target), weight(weight)
        {

        }

        int source;
        int target;
        int weight;
};

// tid fram till 15:00.



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

/// @brief Helper class to implement disjoint sets.
/// supports union and find operations.
class DisjointSet {
    public:
        /// @brief Constructor. Creates an initial structure
        /// with all elements in individual sets.
        /// @param n the number of elements in the set.
        DisjointSet(int n) : n(n), parent(n), rank(n) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        
        /// @brief Finds the root of the set for an element.
        /// also performs path compression during the find, as a side-effect.
        /// Executes in O(log n)
        /// @param elem the element to find the root for.
        /// @return the root of the element. 
        int find_set(int elem) {
            if (elem == parent[elem]) {
                return elem;
            }
            return parent[elem] = find_set(parent[elem]);
        }

        /// @brief Merges the sets of two elements.
        /// uses the rank of the individual sets to optimize the depth
        /// of the resulting tree structure.
        /// Executes in O(log n)
        /// @param elem1 Element one.
        /// @param elem2  Element two.
        void union_sets(int elem1, int elem2) {
            auto a = find_set(elem1);
            auto b = find_set(elem2);
            if (a != b) {
                if (rank[a] < rank[b]) {
                    std::swap(a, b);
                }
                parent[b] = a;
                if (rank[a] == rank[b]) {
                    rank[a]++;
                }
            }
        }

    private:
        int n;
        std::vector<int> parent;
        std::vector<int> rank;
};


std::optional<std::pair<int, std::vector<Edge>>> mst(std::vector<std::vector<Edge>>& graph) {
    std::pair<int, std::vector<Edge>> result;
    DisjointSet disjointSet(graph.size());
    std::vector<Edge> all_edges;

    for (auto& node_vec : graph) {
        for (auto& edge : node_vec) {
            all_edges.push_back(edge);
        }
    }
    std::sort(all_edges.begin(), all_edges.end(), [](Edge& e1, Edge& e2) { return e1.weight < e2.weight;});
    int edge_cost = 0;
    for (auto edge: all_edges) {
        if (disjointSet.find_set(edge.source) != disjointSet.find_set(edge.target)) {
            result.second.push_back(edge);
            disjointSet.union_sets(edge.source, edge.target);
            result.first += edge.weight;
        }
    }
    std::sort(result.second.begin(), result.second.end(), [](Edge& e1, Edge& e2) {
        if (e1.source != e2.source) {
            return e1.source < e2.source;
        }
        return e1.target < e2.target;
    });

    int root_set = disjointSet.find_set(0);
    for (int i = 1; i < graph.size(); i++) {
        if (root_set != disjointSet.find_set(i)) {
            return std::nullopt;
        }
    }

    return result;
}


struct TestCase {
    TestCase(int n, int m) : n(n), m(m), adjacency(n) {

    }

    int n;
    int m;

    bool is_end() 
    {
        return n == 0 && m == 0;
    }

    std::vector<std::vector<Edge>> adjacency;
};

int process_header() 
{
    int N;
    cin >> N;
    return N;
}

TestCase process_input() 
{
    int n, m;
    cin >> n >> m;
    TestCase testCase(n, m);

    if (testCase.is_end()) {
        return testCase;
    }

    int w = 0;
    int u, v;
    for (int i = 0; i < m; i++) {
        cin >> u >> v;
        Edge edge(u - 1, v - 1, w);
        testCase.adjacency[u - 1].push_back(edge);
    }
   return testCase;
}


void print_result(int result) 
{
    std::cout << result << "\n";
}

struct Scc 
{
    public:
    int in_deg;
    int out_deg;
    std::set<int> vertices;
};
// procedure DFS(G, v) is
//     label v as discovered
//     for all directed edges from v to w that are in G.adjacentEdges(v) do
//         if vertex w is not labeled as discovered then
//             recursively call DFS(G, w)

void dfs(int i, std::stack<int>& the_stack, std::vector<bool>& discovered, std::vector<std::vector<Edge>>& adjacency)
{
    discovered[i] = true;
    for (auto edge: adjacency[i]) {
        if (!discovered[edge.target]) {
            dfs(edge.target, the_stack, discovered, adjacency);
        }
    }
    the_stack.push(i);
}

std::vector<std::vector<Edge>> reverse_graph(std::vector<std::vector<Edge>>& adjacency)
{
    std::vector<std::vector<Edge>> reversed(adjacency.size());
    for (auto node : adjacency) {
        for (auto edge: node) {
            Edge reversedEdge(edge.target, edge.source, edge.weight);
            reversed[edge.target].push_back(reversedEdge);
        }
    }
    return reversed;
}

void get_scc(int i, std::set<int>& scc, int scc_id, std::map<int, int>& scc_ids, std::vector<bool>& discovered,
            std::vector<std::vector<Edge>>& adjacency) 
{
    discovered[i] = true;
    scc.insert(i);
    scc_ids[i] = scc_id;
    for (auto edge: adjacency[i]) {
        if (!discovered[edge.target]) {
            get_scc(edge.target, scc, scc_id, scc_ids, discovered, adjacency);
        }
    }
}

int solve(int n, int m, std::vector<std::vector<Edge>>& adjacency) 
{
    std::stack<int> the_stack;
    std::vector<bool> discovered(adjacency.size(), false);
    for (int i = 0; i < n; i++) {
        if (!discovered[i]) {
            dfs(i, the_stack, discovered, adjacency);
        }
    }

    std::vector<std::vector<Edge>> reversed = reverse_graph(adjacency);

    for (int i = 0; i < discovered.size(); i++) {
        discovered[i] = false;
    }
    std::vector<std::set<int>> sccs;
    std::map<int, int> scc_ids;
    while (!the_stack.empty()) {
        int root = the_stack.top();
        the_stack.pop();
        if (!discovered[root]) {
            std::set<int> scc;
            get_scc(root, scc, sccs.size(), scc_ids, discovered, reversed);
            sccs.push_back(scc);
        }
    }

    if (sccs.size() == 1) {
        return 0;
    }

    std::map<int, int> in_counts;
    std::map<int, int> out_counts;

    for (int i = 0; i < sccs.size(); i++) {
        in_counts[i] = 0;
        out_counts[i] = 0;
    }

    for (auto& scc : sccs) {
        for (auto node : scc) {
            for (auto edge: adjacency[node]) {
                if (scc.find(edge.target) == scc.end()) {
                    in_counts[scc_ids[edge.target]]++;
                    out_counts[scc_ids[edge.source]]++;
                }
            }
        }
    }
    
    int in_sets = 0;
    int out_sets = 0;
    for (int i = 0; i < sccs.size(); i++) {
        if (in_counts[i] == 0) {
            in_sets++;
        }
        if (out_counts[i] == 0) {
            out_sets++;
        }
    }
    return std::max(in_sets, out_sets);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int cases = process_header();
    for (int i = 0; i < cases; i++) {
        auto testCase = process_input();
        auto result = solve(testCase.n, testCase.m, testCase.adjacency);
        print_result(result);
    }
    std::cout.flush();
}
