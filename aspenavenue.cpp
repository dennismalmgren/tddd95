#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <ios>
#include <cmath>
#include <iomanip>

std::ifstream cin("in_aspenavenue.txt");
//auto& cin = std::cin;

using Vec = std::vector<int>;

struct TestCase {
    TestCase(int n, int l, int w) : N(n), L(l), W(w), p_vec(n) {
        
    }
    int N;
    int L;
    int W;

    Vec p_vec;
};

TestCase process_input() 
{
    int N, L, W;
    int p;
    cin >> N >> L >> W;
    TestCase testCase(N, L, W);
    for (auto i = 0; i < N; i++) {
        cin >> p;
        testCase.p_vec[i] = p;
    }
    return testCase;
}

auto mindist(int n, int l, int w, Vec& p_vec) 
{
    std::sort(p_vec.begin(), p_vec.end());
    auto l_dist = l * 1.0 / (n/2 - 1);
    auto tree_index = 0;
    double tot_dist = 0;
    for (auto loc_l = 0; loc_l < n / 2; loc_l++) {
        // First do left side
        auto tree_loc = p_vec[tree_index];
        
        auto pos_l = l_dist * loc_l;
        auto pos_r = 0;
        // take the next tree.
        auto tree_p = p_vec[tree_index];
        auto next_tree = p_vec[tree_index + 1];

        if (tree_p <= pos_l) {
            // calculate euclidean distance
            auto dist_l = std::sqrt(std::pow(pos_l - tree_p, 2) + std::pow(pos_r, 2));
            tot_dist += dist_l;
        }
        else {

        }
        tree_index += 1;
        // Then right side.
        pos_r = w;
        tree_p = p_vec[tree_index];
        auto dist_r = std::sqrt(std::pow(pos_l - tree_p, 2) + std::pow(pos_r, 2));
        tot_dist += dist_r;
        tree_index += 1;
    }
    return tot_dist;
}

void print_results(double result) 
{
    std::cout << std::setprecision(11) << result << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    //auto& cin = std::cin;
    cin.tie(nullptr);

    auto testCase = process_input();
    auto result = mindist(testCase.N, testCase.L, testCase.W, testCase.p_vec);
    print_results(result);
}