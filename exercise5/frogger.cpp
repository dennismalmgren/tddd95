#include <bits/stdc++.h>
std::ifstream cin("in_frogger.txt");
//auto& cin = std::cin;

const int CAR = 1;
const int ROAD = 2;

std::map<char, int> map_symbols() 
{
    std::map<char, int> symbols;
    symbols['O'] = ROAD;
    symbols['X'] = CAR;
    return symbols;
}

struct TestCase {
    TestCase(int max_rounds, int n, int m) : max_rounds(max_rounds), n(n), m(m), map(n, std::vector<int>(m)) {
        player_start.first = n - 1;
        goal.first = 0;
    }
    int max_rounds;
    int n;
    int m;
    std::vector<std::vector<int>> map;
    std::pair<int, int> player_start;
    std::pair<int, int> goal;
};

int process_header() {
    int n;
    cin >> n;
    return n;
}

TestCase process_input() 
{
    int max_rounds, n, m;
    char symbol;
    cin >> max_rounds >> n >> m;

    TestCase testCase(max_rounds, n + 2, m);
    auto symbols = map_symbols();
    for (int row = 0; row < n + 2; row++) {
        for (int col = 0; col < m; col++) {
            cin >> symbol;
            if (symbol == 'F') {
                testCase.map[row][col] = ROAD;
                testCase.player_start.second = col;
            }
            else if (symbol == 'G') {
                testCase.map[row][col] = ROAD;
                testCase.goal.second = col;
            }
            else {
                testCase.map[row][col] = symbols[symbol];
            }
        }
    }
    return testCase;
}

struct Node {
    int n;
    int dist;
};

struct Edge {
    int weight;
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


bool is_car(int t, int row, int col, int row_max, int col_thresh, std::vector<std::vector<int>>& map)
{
    int check_col = col;
    if (row != 0 && row != row_max) {
        int transformed_row = row_max - row - 1;
        int shift_value = t;
        if (transformed_row % 2 == 1) { // to the right
            shift_value = -t;
        }
        check_col = (col + shift_value) % col_thresh;
        if (check_col < 0) {
            check_col += col_thresh;
        }
    }
    if (map[row][check_col] == CAR) {
        return true;
    }
    return false;
}

int find_rounds(std::vector<std::vector<int>>& map, 
                std::pair<int, int> player_start, 
                std::pair<int, int> goal,
                int max_rounds) 
{
    int t_max = map[0].size() - 1;
    int row_max = map.size() - 1;
    int col_max = map[0].size() - 1;
    int t_thresh = t_max + 1;
    int row_thresh = row_max + 1;
    int col_thresh = col_max + 1;

    // coordinate scheme:
    auto to_1d = [t_thresh, row_thresh, col_thresh](int t, int row, int col) {
        return t * (row_thresh * col_thresh) + row * (col_thresh) + col;
    };

    auto is_car = [col_thresh, &map, row_max](int t, int row, int col) {
        int check_col = col;
        if (row != 0 && row != row_max) {
            int transformed_row = row_max - row - 1;
            int shift_value = -t;
            if (transformed_row % 2 == 1) { // to the right
                shift_value = t;
            }
            check_col = (col + shift_value) % col_thresh;
            if (check_col < 0) {
                check_col += col_thresh;
            }
        }
        if (map[row][check_col] == CAR) {
            return true;
        }
        return false;
    };

    int start = to_1d(0, player_start.first, player_start.second);

    int nodes = t_thresh * row_thresh * col_thresh;
    std::vector<std::vector<Edge>> graph(nodes);
    std::vector<int> goals;
    for (int t = 0; t <= t_max; t++) {
        goals.push_back(to_1d(t, goal.first, goal.second));

        for (int row = 0; row <= row_max; row++) {
            for (int col = 0; col <= col_max; col++) {
                int node_id = to_1d(t, row, col);
                if (is_car(t, row, col)) {
                    continue;
                }

                int t_next = (t + 1) % t_thresh;
                std::array<std::pair<int, int>, 5> edges = {
                    std::make_pair(row + 1, col),
                    std::make_pair(row - 1, col),
                    std::make_pair(row, col + 1),
                    std::make_pair(row, col - 1),
                    std::make_pair(row, col)   
                };

                for (auto edge: edges) {
                    if (edge.first < 0 
                    || edge.first > row_max 
                    || edge.second < 0 
                    || edge.second > col_max) {
                        continue;
                    }
                    if (is_car(t_next, edge.first, edge.second)) {
                        continue;
                    }

                    Edge the_edge;
                    the_edge.target = to_1d(t_next, edge.first, edge.second);
                    the_edge.weight = 1;
                    graph[to_1d(t, row, col)].push_back(the_edge);
                }
            }
        }
    }

    auto parents_dists = shortest_path(graph, start);
    int min_dist = std::numeric_limits<int>::max();
    for (auto goal_node : goals) {
        min_dist = std::min(min_dist, parents_dists.first[goal_node]);
    }

    if (min_dist > max_rounds) {
        min_dist = std::numeric_limits<int>::max();
    }
    return min_dist;
}

void print_result(int result) 
{
    if (result == std::numeric_limits<int>::max()) {
        std::cout << "The problem has no solution.\n";
    }
    else {
        std::cout << "The minimum number of turns is " << result << ".\n";
    }
}



int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n_scenarios = process_header();
    for (int i = 0; i < n_scenarios; i++) {
        auto testCase = process_input();
        auto result = find_rounds(testCase.map, testCase.player_start, testCase.goal, testCase.max_rounds);
        print_result(result);
    }
    std::cout.flush();
}