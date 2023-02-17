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

    for (int t_eval = max_t; t_eval >= 1; t_eval--) {
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < n; y++) {
                auto xy_map = t_xy_map[t_eval];

                std::array<int, 16> travel_values({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
                int spawn_bonus = 0;
                if (xy_map.find(to_1d(x, y, n)) != xy_map.end()) {
                    for (int i = 0; i < 16; i++) {
                        travel_values[i]++;
                    }
                    spawn_bonus = 1;
                }
                for (int d_taken = 1; d_taken <= d; d_taken++) {
                    auto x_left = x - d_taken;
                    auto x_right = x + d_taken;
                    auto y_up = y - d_taken;
                    auto y_down = y + d_taken;
                    if (x_left >= 0) {
                        if (xy_map.find(to_1d(x_left, y, n)) != xy_map.end()) {
                            travel_values[0]++;
                        }
                        state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                                    travel_values[0] + state[t_eval + 1][x_left][y]);
                    }
                    if (x_right < n) {
                        if (xy_map.find(to_1d(x_right, y, n)) != xy_map.end()) {
                            travel_values[1]++;
                        }
                        state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                                    travel_values[1] + state[t_eval + 1][x_right][y]);

                    }
                    if (y_up >= 0) {
                        if (xy_map.find(to_1d(x, y_up, n)) != xy_map.end()) {
                            travel_values[2]++;
                        }
                        state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                                    travel_values[2] + state[t_eval + 1][x][y_up]);
                    }
                    if (y_down < n) {
                        if (xy_map.find(to_1d(x, y_down, n)) != xy_map.end()) {
                            travel_values[3]++;
                        }
                        state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                                    travel_values[3] + state[t_eval + 1][x][y_down]);
                    }

                    if (d_taken == 2 || d_taken == 3 || d_taken == 5) {
                        // add diagonals
                        auto dist = std::floor(std::sqrt(d_taken * d_taken / 2));
                        x_left = x - dist;
                        x_right = x + dist;
                        y_up = y - dist;
                        y_down = y + dist;
                        if (x_left >= 0 && y_up >= 0) {
                            if (xy_map.find(to_1d(x_left, y_up, n)) != xy_map.end()) {
                                travel_values[4]++;
                            }
                            state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                                    travel_values[4] + state[t_eval + 1][x_left][y_up]);
                        }
                        if (x_left >= 0 && y_down < n) {
                            if (xy_map.find(to_1d(x_left, y_down, n)) != xy_map.end()) {
                                travel_values[5]++;
                            }
                            state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                    travel_values[5] + state[t_eval + 1][x_left][y_down]);
                        }
                        if (x_right < n && y_up >= 0) {
                            if (xy_map.find(to_1d(x_right, y_up, n)) != xy_map.end()) {
                                travel_values[6]++;
                            }
                            state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                travel_values[6] + state[t_eval + 1][x_right][y_up]);
                        }
                        if (x_right < n && y_down < n) {
                            if (xy_map.find(to_1d(x_right, y_down, n)) != xy_map.end()) {
                                travel_values[7]++;
                            }
                            state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                travel_values[7] + state[t_eval + 1][x_right][y_down]);
                        }
                    }
                    if (d_taken == 3 || d_taken == 5) {
                        // add horse moves
                        auto x_step = d_taken - 1;
                        auto y_step = x_step / 2;
                        auto travel_values_offset = 0;
                        for (int i = 0; i < 2; i++) {
                            if (i == 1) {
                                travel_values_offset += 4;
                                std::swap(x_step, y_step);
                            }

                            if (x - x_step >= 0 && y - y_step >= 0) {
                                 if (xy_map.find(to_1d(x - x_step, y - y_step, n)) != xy_map.end()) {
                                    travel_values[8 + travel_values_offset]++;
                                }
                                state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                    travel_values[8 + travel_values_offset] + state[t_eval + 1][x - x_step][y - y_step]);
                            }

                            if (x - x_step >= 0 && y + y_step < n) {
                                 if (xy_map.find(to_1d(x - x_step, y + y_step, n)) != xy_map.end()) {
                                    travel_values[9 + travel_values_offset]++;
                                }
                                state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                    travel_values[9 + travel_values_offset] + state[t_eval + 1][x - x_step][y + y_step]);
                            }

                            if (x + x_step < n && y - y_step >= 0) {
                                 if (xy_map.find(to_1d(x + x_step, y - y_step, n)) != xy_map.end()) {
                                    travel_values[10 + travel_values_offset]++;
                                }
                                state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                    travel_values[10 + travel_values_offset] + state[t_eval + 1][x + x_step][y - y_step]);
                            }

                            if (x + x_step < n && y + y_step < n) {
                                 if (xy_map.find(to_1d(x + x_step, y + y_step, n)) != xy_map.end()) {
                                    travel_values[11 + travel_values_offset]++;
                                }
                                state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                    travel_values[11 + travel_values_offset] + state[t_eval + 1][x + x_step][y + y_step]);
                            }
                        }
                    }

                    if (d_taken == 4) {
                        // (3, 1), (3, 2)
                        for (int i = 0; i < 2; i++) {
                            auto large_step = d_taken - 1;
                            for (auto small_step = 1; small_step <= 2; small_step++) {
                                auto x_step = large_step;
                                auto y_step = small_step;
                                if (i == 1) {
                                    std::swap(x_step, y_step);
                                }

                                if (x - x_step >= 0 && y - y_step >= 0) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x - x_step, y - y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x - x_step][y - y_step]);
                                }

                                if (x - x_step >= 0 && y + y_step < n) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x - x_step, y + y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x - x_step][y + y_step]);
                                }

                                if (x + x_step < n && y - y_step >= 0) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x + x_step, y - y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x + x_step][y - y_step]);
                                }

                                if (x + x_step < n && y + y_step < n) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x + x_step, y + y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x + x_step][y + y_step]);
                                }
                            }   
                        }
                    }
                 // remains: special cases.
                    if (d_taken == 5) {
                        // (3, 1), (3, 2)
                        for (int i = 0; i < 2; i++) {
                            auto large_step = d_taken - 1;
                            for (auto small_step = 1; small_step <= 3; small_step++) {
                                if (small_step == 2) {
                                    continue;
                                }
                                auto x_step = large_step;
                                auto y_step = small_step;
                                if (i == 1) {
                                    std::swap(x_step, y_step);
                                }

                                if (x - x_step >= 0 && y - y_step >= 0) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x - x_step, y - y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x - x_step][y - y_step]);
                                }

                                if (x - x_step >= 0 && y + y_step < n) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x - x_step, y + y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x - x_step][y + y_step]);
                                }

                                if (x + x_step < n && y - y_step >= 0) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x + x_step, y - y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x + x_step][y - y_step]);
                                }

                                if (x + x_step < n && y + y_step < n) {
                                    auto step_value = spawn_bonus;
                                    if (xy_map.find(to_1d(x + x_step, y + y_step, n)) != xy_map.end()) {
                                        step_value += 1;
                                    }
                                    state[t_eval][x][y] = std::max(state[t_eval][x][y],
                                        step_value + state[t_eval + 1][x + x_step][y + y_step]);
                                }
                            }   
                        }
                    }
                }
                max_val[t_eval] = std::max(max_val[t_eval], state[t_eval][x][y]);
            }
        }
    }
    return max_val[1];
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