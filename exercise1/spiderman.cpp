#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <ios>
#include <limits>
#include <numeric>

std::ifstream cin("in_spiderman.txt");
//auto& cin = std::cin;
using Vec = std::vector<int>;

struct TestCase {
    TestCase(int n) : n(n), sequence(n){
        
    }
    int n;
    Vec sequence;
};

int process_header() {
    int N;
    cin >> N;
    return N;
}

std::optional<TestCase> process_input() 
{
    int n;
    int number;
    
    if (cin >> n) {
        TestCase testCase(n);
        for (int i = 0; i < n; i++) {
            cin >> number;
            testCase.sequence[i] = number;
        }
        return testCase;
    }
    return std::nullopt;
}

void print_results(const Vec& results) 
{
    if (results.size() == 0) {
        std::cout << "IMPOSSIBLE";
    }
    else {
        for (auto index: results) {
            std::cout << (index == 0 ? "D" : "U");
        }
    }
    std::cout << std::endl;
}
// Keep track of the best actions so far.
auto climb(const Vec& sequence) 
{
    auto n = sequence.size();
    const int max_climb_height = std::accumulate(sequence.begin(), sequence.end(), 0) / 2; // could do sum(sequence) / 2.
    std::vector<std::vector<int>> achieved_heights(n, std::vector<int>(max_climb_height + 1, max_climb_height + 1));
    std::vector<std::vector<int>> actions(n, std::vector<int>(max_climb_height + 1, max_climb_height + 1)); // 0 for down, 1 for up
    std::vector<std::vector<int>> height_inds(n, std::vector<int>(max_climb_height + 1, max_climb_height + 1)); // 0 for down, 1 for up
    achieved_heights[0][sequence[0]] = sequence[0];
    height_inds[0][sequence[0]] = sequence[0];
    for (auto i = 1; i < n; i++) {
        for (auto height = 0; height <= max_climb_height; height++) {
            if (achieved_heights[i - 1][height] <= max_climb_height) {
                // option 1: we reached a new 'maximum' and can't avoid it. (cannot happen for climb down)
                // option 2: we've reached this path multiple times and found a better max height
                // option 3: we've reached this height but along a path with higher max height
                auto o_up = height + sequence[i];
                if (o_up <= max_climb_height) {
                    auto o2 = achieved_heights[i - 1][height];
                    auto o3 = achieved_heights[i][o_up];
                    auto climb_result = std::max(o_up, std::min(o2, o3));
                    if (climb_result == o_up || climb_result == o2) {
                        //actions[i][climb_result] = 1;
                        height_inds[i][o_up] = height;
                        actions[i][o_up] = 1;
                    }
                    achieved_heights[i][o_up] = climb_result;
                }

                auto o_down = height - sequence[i];
                if (o_down >= 0) {
                    auto o2 = achieved_heights[i - 1][height];
                    auto o3 = achieved_heights[i][o_down];
                    auto climb_result = std::min(o2, o3);
                    if (climb_result == o2) {
                        actions[i][o_down] = 0;
                        height_inds[i][o_down] = height;
                    }

                    achieved_heights[i][o_down] = climb_result;
                }

            }
        }
    }

    // first of all, achieved_heights[0][n - 1] needs to be 1.
    // second of all, we need to track the best solution.
    // maybe begin with figuring out the max height.
    if (achieved_heights[n - 1][0] == max_climb_height + 1) {
        return std::vector<int>();
    }
    auto current_height = 0;
    auto climb_ind = height_inds[n - 1][0];

    std::vector<int> action_result;
    action_result.push_back(actions[n - 1][0]);
    for (auto i = n - 1; i > 1; i--) {
        action_result.push_back(actions[i - 1][climb_ind]);
         climb_ind = height_inds[i - 1][climb_ind];
    }
    action_result.push_back(1);
    std::reverse(action_result.begin(), action_result.end());
    return action_result;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    bool new_test_case = true;
    process_header(); // Throw it away.
    while (auto testCase = process_input()) {
        auto& testCaseVal = *testCase;
        auto test_case_results = climb(testCaseVal.sequence);
        print_results(test_case_results);
    }
}
