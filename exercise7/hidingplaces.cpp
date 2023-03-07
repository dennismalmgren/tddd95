//distance/parent[] shortest_path(graph G, node start)
#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_hidingplaces.txt");
//auto& cin = std::cin;

using namespace algo;

std::string to_stringrep(int coord) 
{
    int row_coord = coord / 8;
    int col_coord = coord % 8;

    char col = 'a' + col_coord;
    char row = '1' + row_coord;
    return std::string(1, col) + std::string(1, row) ;
}

int from_stringrep(std::string pos)
{
    char first = pos[0];
    int col_coord = first - 'a';
    char second = pos[1];
    int row_coord = second - '1';
    return row_coord * 8 + col_coord;
}

int process_header()
{
    int n; 
    cin >> n;
    return n;
}

int process_input() 
{
    std::string testcase;
    cin >> testcase;
    return from_stringrep(testcase);
}

void print_result(int dist, std::vector<int>& results) 
{
    std::cout << dist << " ";
    std::vector<std::string> string_results;

    for (auto& result : results) {
        string_results.push_back(to_stringrep(result));
    }
    std::sort(string_results.begin(), string_results.end(), [](const std::string& a, const std::string& b) {
        if (a[1] > b[1]) {
            return true;
        }
        else if (a[1] < b[1]) {
            return false;
        }
        else {
            return a[0] < b[0];
        }
    });
    for (auto& string_result : string_results) {
        std::cout << string_result << " ";
    }
    std::cout << "\n";
}

using ChessNode = DistNode<int>;

std::vector<int> shortest_paths(
                    int start)
{
    const int MAX_DIST = std::numeric_limits<int>::max();
    int n = 8;
    int graph_size = n * n;
    
    auto comp = [](const ChessNode& a, const ChessNode& b ) { 
        if (a.dist != b.dist) {
            return a.dist > b.dist; 
        }
        else { 
            return a.n > b.n;
        }
    };

    std::vector<int> distances(graph_size, MAX_DIST);
    std::vector<bool> visited(graph_size, false);

    std::priority_queue<ChessNode, std::vector<ChessNode>, decltype(comp)> queue(comp);

    distances[start] = 0;
    queue.push(ChessNode{start, 0});
    while (!queue.empty()) {
        auto node = queue.top();
        queue.pop();
        if (visited[node.n]) {
            continue;
        }
        int row = node.n / 8;
        int col = node.n % 8;

        std::array<std::pair<int, int>, 8> next_nodes = {
            std::make_pair(row - 2, col - 1),
            std::make_pair(row - 2, col + 1),
            std::make_pair(row - 1, col - 2),
            std::make_pair(row - 1, col + 2),
            std::make_pair(row + 1, col - 2),
            std::make_pair(row + 1, col + 2),
            std::make_pair(row + 2, col - 1),
            std::make_pair(row + 2, col + 1),            
        };

        for (auto& next_node : next_nodes)
        {
            if (next_node.first < 0 || next_node.first > 7
                || next_node.second < 0 || next_node.second > 7) {
                continue;
            }

            int next_node_index = next_node.first * 8 + next_node.second;
            if (distances[next_node_index] > distances[node.n] + 1) {
                distances[next_node_index] = distances[node.n] + 1;
                queue.push(ChessNode{next_node_index, distances[next_node_index]});
            }
        }
        visited[node.n] = true;
    }

    return distances;
}

std::pair<int, std::vector<int>> solve(int start)
{
    auto distances = shortest_paths(start);
    int max_dist = 0;
    for (auto dist: distances) {
        max_dist = std::max(max_dist, dist);
    }
    std::vector<int> results;
    for (int i = 0; i < distances.size(); i++) {
        if (distances[i] == max_dist) {
            results.push_back(i);
        }
    }

    // for (int i = 0; i < distances.size(); i++) {
    //     std::cout << to_stringrep(i) << " " << distances[i] << "\n";
    // }

    return std::make_pair(max_dist, results);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int num_testcases = process_header();
    for (int i = 0; i < num_testcases; i++) {
        auto testcase = process_input();
        auto solution = solve(testcase);
        print_result(solution.first, solution.second);
    }

    std::cout.flush();
}