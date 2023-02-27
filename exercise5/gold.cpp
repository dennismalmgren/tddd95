#include <bits/stdc++.h>
std::ifstream cin("in_gold.txt");
//auto& cin = std::cin;

const int GOLD = 1;
const int TRAP = 2;
const int WALL = 3;
const int FLOOR = 4;
const int PLAYER = 0;

std::map<char, int> map_symbols() 
{
    std::map<char, int> symbols;
    symbols['G'] = GOLD;
    symbols['T'] = TRAP;
    symbols['#'] = WALL;
    symbols['.'] = FLOOR;
    return symbols;
}


struct TestCase {
    TestCase(int w, int h) : W(w), H(h), map(h, std::vector<int>(w)) {
        
    }
    int W;
    int H;
    std::vector<std::vector<int>> map;
    int player_y;
    int player_x;
};

TestCase process_input() 
{
    int w, h;
    char symbol;
    cin >> w >> h;
    TestCase testCase(w, h);
    auto symbols = map_symbols();

    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            cin >> symbol;
            if (symbol == 'P') {
                testCase.map[row][col] = FLOOR;
                testCase.player_x = col;
                testCase.player_y = row;
            }
            else {
                testCase.map[row][col] = symbols[symbol];
            }
        }
    }
    return testCase;
}

void print_result(int result) 
{
    std::cout << result << "\n";
}

struct Node
{
    public:
        std::pair<int, int> location;

};

bool is_safe(std::pair<int, int> location, const std::vector<std::vector<int>>& map) 
{
    if (location.first < map.size() - 1) {
        if (map[location.first + 1][location.second] == TRAP) {
            return false;
        }
    }

    if (location.first > 0) {
        if (map[location.first - 1][location.second] == TRAP) {
            return false;
        }
    }

    if (location.second < map[0].size() - 1) {
        if (map[location.first][location.second + 1] == TRAP) {
            return false;
        }
    }

    if (location.second > 0) {
        if (map[location.first][location.second - 1] == TRAP) {
            return false;
        }
    }

    return true;
}

int find_gold(std::vector<std::vector<int>>& map, int player_x, int player_y) 
{
    auto gold_collected = 0;
    std::vector<std::vector<bool>> explored(map.size(), std::vector<bool>(map[0].size(), false));
    //std::vector<std::vector<bool>> gold_discovered(map.size(), std::vector<bool>(map[0].size(), false));
    //std::vector<std::pair<int, int>> gold_locations;
    std::queue<std::pair<int, int>> queue;
    auto root = std::make_pair(player_y, player_x);
    explored[root.first][root.second] = true;
    queue.push(root);
    while (queue.size() > 0) {
        auto location = queue.front();
        queue.pop();
        if (map[location.first][location.second] == WALL) {
            continue;
        }
        if (map[location.first][location.second] == GOLD) {
        //    gold_discovered[location.first][location.second] = true;
        //    gold_locations.push_back(std::make_pair(location.first, location.second));
            gold_collected++;
        }

        if (is_safe(location, map)) {
            if (location.first < map.size() - 1 && !explored[location.first + 1][location.second]) {
                explored[location.first + 1][location.second] = true;
                queue.push(std::make_pair(location.first + 1, location.second));
            }

            if (location.first > 0 && !explored[location.first - 1][location.second]) {
                explored[location.first - 1][location.second] = true;
                queue.push(std::make_pair(location.first - 1, location.second));
            }

            if (location.second < map[0].size() - 1 && !explored[location.first][location.second + 1]) {
                explored[location.first][location.second + 1] = true;
                queue.push(std::make_pair(location.first, location.second + 1));
            }

            if (location.second > 0 && !explored[location.first][location.second - 1]) {
                explored[location.first][location.second - 1] = true;
                queue.push(std::make_pair(location.first, location.second - 1));
            }
        }
    }
    return gold_collected;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    auto result = find_gold(testCase.map, testCase.player_x, testCase.player_y);
    print_result(result);
    std::cout.flush();
}