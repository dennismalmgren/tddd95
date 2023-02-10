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
#include <iomanip>
#include <cmath>

std::ifstream cin("in_aspen_avenue.txt");
//auto& cin = std::cin;
using Vec = std::vector<int>;

struct TestCase {
    TestCase(int n, int l, int w) : N(n), L(l), W(w), p_vec(n){
        
    }
    int N;
    int L;
    int W;

    int n;
    Vec p_vec;
};

TestCase process_input() 
{
    int n, l, w;
    int p;
    int number;
    cin >> n >> l >> w;
    TestCase testCase(n, l, w);
    for (auto i = 0; i < n; i++) {
        cin >> p;
        testCase.p_vec[i] = p;
    }
    return testCase;
}

void print_results(const double result) 
{
    std::cout << std::setprecision(10) << result << std::endl;
}

void calculate_distances(std::vector<std::vector<double>>& distances, 
                        int n, int l, int w, Vec& p_vec) {
    auto l_dist = 1.0 * l / (n/2 - 1);
    auto w_dist = std::pow(w, 2);
    for (auto tree_index = 0; tree_index < n; tree_index++) {
        for (auto i = 0; i < n; i+= 2) {
            auto pow_dist = std::pow(p_vec[tree_index] - i / 2 * l_dist, 2);
            distances[tree_index][i] = std::sqrt(pow_dist);
            distances[tree_index][i + 1] = std::sqrt(pow_dist + w_dist);
        }
    }
}

double shift_trees(const std::vector<std::vector<double>>& distances, 
                    std::vector<std::vector<double>>& cost,
                    int tree_count, 
                    int trees_to_shift) {

    for (auto tree_index = 0; tree_index < tree_count; tree_index++) {

        //what are the indices.
        // left side distances:
        // for tree nr 0 (1), with 0 shifted, we have distance to 0.
        // for tree nr 1 (2), with 0 shifted, we have distance to 2.
        // for tree nr 3 (3), with 0 shifted, we have distance to 6.
        // for tree nr 1 (2), with 1 shifted, we have distance to 0.
        // for tree nr 3 (4), with 1 shifted, we have distance to 2
        // (tree - shifted) * 2 ?

        // for tree nr 0 (1), with 1 shifted, we have distance to 1.
        // for tree nr 1 (2), with 1 shifted, we have distance to 1.
        // for tree nr 1 (2), with 1 shifted, we have distance to 1.
        // for tree nr 3 (4), with 1 shifted, we have distance to 1
        // for tree nr 3 (4), with 2 shifted, we have distance to 3.
        // for tree nr 3 (4), with 3 shifted, we have distance to 5.
        // trees_shifted * 2 - 1
        for (auto trees_shifted = std::max(0, tree_index - trees_to_shift); 
                trees_shifted <= std::min(trees_to_shift, tree_index + 1); trees_shifted++) {
                    if (tree_index == 0 && trees_shifted == 0) {
                        cost[tree_index][trees_shifted] = distances[tree_index][trees_shifted];
                    }
                    else if (trees_shifted == 0) {
                        cost[tree_index][trees_shifted] = distances[tree_index][tree_index * 2] + cost[tree_index - 1][trees_shifted];
                    }
                    else if (tree_index == 0 && trees_shifted == 1) {
                        // there is only one option.
                        cost[tree_index][trees_shifted] = distances[tree_index][trees_shifted * 2 - 1];
                    }
                    else if (tree_index == trees_shifted - 1) {
                        // shift all the trees.
                        auto cost_for_shift = distances[tree_index][trees_shifted * 2 - 1] + cost[tree_index - 1][trees_shifted - 1];
                        cost[tree_index][trees_shifted] = cost_for_shift;
                    }
                    else {
                        // We either shift the required trees previously
                        auto cost_for_not_shift = distances[tree_index][(tree_index - trees_shifted) * 2] + cost[tree_index - 1][trees_shifted];
                        // or we shift this tree.
                        auto cost_for_shift = distances[tree_index][trees_shifted * 2 - 1] + cost[tree_index - 1][trees_shifted - 1];
                        cost[tree_index][trees_shifted] = std::min(cost_for_not_shift, cost_for_shift);
                    }

        }
    }
  
    return cost[tree_count - 1][trees_to_shift];
}

// Keep track of the best actions so far.
auto place(int n, int l, int w, Vec& p_vec) 
{
    std::sort(p_vec.begin(), p_vec.end());
    //holes are indexed 1-2, 3-4 from top to bottom.
    std::vector<std::vector<double>> distances(n, std::vector<double>(n));
    calculate_distances(distances, n, l, w, p_vec);
    std::vector<std::vector<double>> cost(n, std::vector<double>(n / 2 + 1, -1)); //

    double result = shift_trees(distances, cost, n, n / 2);
    
    return result;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    auto test_case_results = place(testCase.N, testCase.L, testCase.W, testCase.p_vec);
    print_results(test_case_results);
}
