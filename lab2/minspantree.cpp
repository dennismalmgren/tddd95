// minspantree.cpp
// Author: Dennis Malmgren
// Finds the minimum spanning tree in a graph
#include <bits/stdc++.h>


// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_minspantree.txt");
//auto& cin = std::cin;

/// @brief Helper class, represents an edge in a graph.
/// Keeps track of the nodes it connects, and the weight.
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


// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph.
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


/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
TestCase process_input() 
{
    int n, m;
    cin >> n >> m;
    TestCase testCase(n, m);
    if (testCase.is_end()) {
        return testCase;
    }

    int u, v, w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        if (v < u) {
            std::swap(v, u);
        }
        Edge edge(u, v, w);
        testCase.adjacency[u].push_back(edge);
    }
   return testCase;
}

/// @brief Prints the results of finding an MST.
/// @param result If result is std::nullopt, the method writes "Impossible".
/// Otherwise, the total weight of the MST and the used edges are printed.
void print_result(std::optional<std::pair<int, std::vector<Edge>>>& result) 
{
    if (!result.has_value()) {
        std::cout << "Impossible\n";
    }
    else {
        auto value = result.value();
        std::cout << value.first << "\n";
        for (auto edge: value.second) {
            std::cout << edge.source << " " << edge.target << "\n";
        }
    }
}

/// @brief Algorithm to calculate the minimum spanning tree. 
/// Time complexity: O(E log E)
/// @tparam T the type of values in the graph
/// @param graph adjacency list representation of a graph.
/// @return std::nullopt if there is no MST. Otherwise a pair, 
/// where the first element is the total weight of the tree and the second
/// element is the edges of the tree.
std::optional<std::pair<int, std::vector<Edge>>> mst(
    std::vector<std::vector<Edge>>& graph) {
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

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true) {
        auto testCase = process_input();
        if (testCase.is_end()) {
            break;
        }
        auto result = mst(testCase.adjacency);
        print_result(result);
    }
    std::cout.flush();
}