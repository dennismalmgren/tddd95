#include <bits/stdc++.h>
std::ifstream cin("in_whacamole.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase(int n, int d, int m) : n(n), d(d), m(m), x(m), y(m), t(m) {
        
    }

    int n;
    int d;
    int m;
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> t;
};

TestCase process_input() 
{
    int n;
    int d;
    int m;
    int x;
    int y; 
    int t;
    cin >> n >> d >> m;
    TestCase testCase(n, d, m);

    for (auto i = 0; i < m; i++) {
        cin >> x >> y >> t;
        testCase.x[i] = x;
        testCase.y[i] = y;
        testCase.t[i] = t;
    }
    return testCase;
}

int to_1d(int x, int y, int n) {
    return x + y * n;
}

double move_dist(std::pair<int, int>& action) {
    return std::sqrt(action.first * action.first + action.second * action.second);
}
long long gcd(long long a, long long b) 
{
    return b == 0 ? a : gcd(b, a % b);
}
int whac(int n, int d, std::vector<int>& x, std::vector<int>& y, std::vector<int>& t) {
    // vector of t, x, y

    std::map<int, std::vector<int>> t_x;
    std::map<int, std::vector<int>> t_y;
    std::map<int, std::set<int>> t_xy_map;
    // Transform input to time-sorted appearances.
    int max_t = -1;
    for (int i = 0; i < t.size(); i++) {
        if (t_x.find(t[i]) == t_x.end()) {
            t_x[t[i]] = std::vector<int>();
        }
        if (t_y.find(t[i]) == t_y.end()) {
            t_y[t[i]] = std::vector<int>();
        }
        
        if (t_xy_map.find(t[i]) == t_xy_map.end()) {
            t_xy_map[t[i]] = std::set<int>();
        }
        t_x[t[i]].push_back(x[i]);
        t_y[t[i]].push_back(y[i]);
        t_xy_map[t[i]].insert(to_1d(x[i], y[i], n));
        max_t = std::max(max_t, t[i]);
    }

    std::vector<std::vector<std::vector<int>>> state(max_t + 2, 
            std::vector<std::vector<int>>(n,
            std::vector<int>(n)));


    std::vector<int> max_val(max_t + 1);

    // // Create value map for final time step
    std::vector<std::pair<int, int>> actions;
    for (int dx = 0; dx * dx <= d * d; dx++) {
        for (int dy = 0; dy * dy + dx * dx <= d * d; dy++) {
            // Keep the shortest actions.
            if (gcd(dx, dy) != 1) {
                continue;
            }
            actions.push_back(std::make_pair(dx, dy));
            if (dx != 0) {
                actions.push_back(std::make_pair(-dx, dy));
            }
            if (dy != 0) {
                actions.push_back(std::make_pair(dx, -dy));
            }
            if (dy != 0 && dx != 0) {
                actions.push_back(std::make_pair(-dx, -dy));
            }
        }
    }

    for (int t_eval = 1; t_eval <= max_t; t_eval++) {
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < n; y++) {
                auto xy_map = t_xy_map[t_eval];
                int spawn_bonus = 0;
                if (xy_map.find(to_1d(x, y, n)) != xy_map.end()) {
                    spawn_bonus++;
                }
                
                for (auto action: actions) {
                    auto action_hits = spawn_bonus;
                    auto new_x = x + action.first;
                    auto new_y = y + action.second;
                    auto distance = move_dist(action);
                    while (distance <= d && new_x >= 0 && new_x < n && new_y >= 0 && new_y < n) {
                        if (xy_map.find(to_1d(new_x, new_y, n)) != xy_map.end()) {
                            action_hits++;
                        }
                        state[t_eval][new_x][new_y] = std::max(state[t_eval][new_x][new_y],
                                                    action_hits + state[t_eval - 1][x][y]);

                        new_x += action.first;
                        new_y += action.second;
                        distance += move_dist(action); 
                    }
                }
            }
        }
    }
    auto first_max_val = 0;
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            first_max_val = std::max(first_max_val, state[max_t][x][y]);
        }
    }
    return first_max_val;
}

void print_result(int result) 
{
    std::cout << result;
    std::cout << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true) {
        auto testCase = process_input();
        if (testCase.n == testCase.m && testCase.m == testCase.d && testCase.n == 0) {
            break;
        }
        auto result = whac(testCase.n, testCase.d, testCase.x, testCase.y, testCase.t);
        print_result(result);
    }

    std::cout.flush();
}