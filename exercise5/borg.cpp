#include <bits/stdc++.h>
std::ifstream cin("in_borg.txt");
//auto& cin = std::cin;

const int OPEN = 1;  //' '
const int WALL = 2;  //'#'


std::map<char, int> map_symbols() 
{
    std::map<char, int> symbols;
    symbols[' '] = OPEN;
    symbols['#'] = WALL;

    return symbols;
}

struct TestCase {
    TestCase(int x, int y) : x(x), y(y), map(y, std::vector<int>(x)) {

    }

    int x;
    int y;

    std::vector<std::vector<int>> map;
    std::pair<int, int> s;
    std::vector<std::pair<int, int>> aliens;
};

int process_header() 
{
    int N;
    cin >> N;
    return N;
}

TestCase process_input() 
{
    int x, y;
    char symbol;
    cin >> std::skipws >> x >> y >> std::noskipws >> symbol;
    TestCase testCase(x, y);

    auto symbols = map_symbols();
    for (int row = 0; row < y; row++) {
        for (int col = 0; col < x; col++) {
            cin >> std::noskipws >> symbol;
            if (symbol == 'S') {
                testCase.s.first = row;
                testCase.s.second = col;
                testCase.map[row][col] = OPEN;
            }
            else if (symbol == 'A') {
                testCase.aliens.push_back(std::make_pair(row, col));
                testCase.map[row][col] = OPEN;
            }
            else {
                testCase.map[row][col] = symbols[symbol];
            }
        }
        cin >> std::noskipws >> symbol;
    }
    return testCase;
}

struct Node {
    int n;
    int dist;
};

struct Edge {
    int weight;
    int source;
    int target;
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


void print_result(int result) 
{
    std::cout << result << "\n";
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


int solve(std::vector<std::vector<int>>& map,
           std::pair<int, int> s,
           std::vector<std::pair<int, int>> aliens) 
{
    int rows = map.size();
    int cols = map[0].size();

    auto to_1d = [rows, cols](int row, int col) {
        return row * cols + col;
    };
    
    std::vector<std::vector<Edge>> graph(rows * cols);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (map[row][col] == WALL) {
                continue;
            }

            std::array<std::pair<int, int>, 4> edges = {
                std::make_pair(row + 1, col),
                std::make_pair(row - 1, col),
                std::make_pair(row, col + 1),
                std::make_pair(row, col - 1),
            };

            for (auto edge: edges) {
                if (edge.first < 0 
                    || edge.first >= rows 
                    || edge.second < 0 
                    || edge.second >= cols) {
                        continue;
                    }

                if (map[edge.first][edge.second] == WALL) {
                    continue;
                }

                Edge the_edge;
                the_edge.source = to_1d(row, col);
                the_edge.target = to_1d(edge.first, edge.second);
                the_edge.weight = 1;
                graph[the_edge.source].push_back(the_edge);
            }
        }
    }
    std::vector<int> points;
    int s_point_index = to_1d(s.first, s.second);
    points.push_back(s_point_index);
    std::map<int, int> new_point_index;
    new_point_index[s_point_index] = 0;
    int point_counter = 0;
    for (auto point : aliens) {
        int a_point_index = to_1d(point.first, point.second);
        point_counter++;
        points.push_back(a_point_index);
        new_point_index[a_point_index] = point_counter;
    }
    std::vector<std::vector<Edge>> newEdges(points.size());
    for (auto point: points) {
        auto s_shortest_path = shortest_path(graph, point);
        for (auto point2 : points) {
            if (point == point2) {
                continue;
            }
            Edge the_edge;
            the_edge.source = new_point_index[point];
            the_edge.target = new_point_index[point2];
            the_edge.weight = s_shortest_path.first[point2];
            newEdges[the_edge.source].push_back(the_edge);
        }
    }

    auto result = mst(newEdges);
    if (result.has_value()) {
        return result.value().first;
    } 
    return 0;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int cases = process_header();
    for (int i = 0; i < cases; i++) {
        auto testCase = process_input();
        auto result = solve(testCase.map, testCase.s, testCase.aliens);
        print_result(result);
    }
    std::cout.flush();
}