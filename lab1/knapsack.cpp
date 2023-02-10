#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <ios>

std::ifstream cin("in.txt");
//auto& cin = std::cin;

using Vec = std::vector<int>;

struct TestCase {
    TestCase(int c, int n) : C(c), weights(n), values(n) {
        
    }
    int C;
    Vec weights;
    Vec values;
};

std::optional<TestCase> process_input() 
{
    int c, n, weight, value;
    
    if (cin >> c >> n) {
        TestCase testCase(c, n);
        for (int i = 0; i < n; i++) {
            cin >> value >> weight;
            testCase.weights[i] = weight;
            testCase.values[i] = value;
        }
        return testCase;
    }
    return std::nullopt;
}

auto knapsack(int capacity, Vec& values, Vec& weights) 
{
    auto n = values.size();
    std::vector<std::vector<int>> K(n + 1, std::vector<int>(capacity + 1));
     
    for (auto i = 0; i <= n; i++) {
        for (auto w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                K[i][w] = 0;
            }
            else if (weights[i - 1] <= w) {
                K[i][w] = std::max(values[i - 1] + K[i - 1][w - weights[i - 1]], K[i - 1][w]);
            }
            else {
                K[i][w] = K[i - 1][w];
            }
        }
    }
    
    // Backtrack to find the actual items.
    auto remaining = K[n][capacity];
    auto w = capacity;
    std::vector<int> items;
    for (auto i = n; i >= 0 && remaining > 0; i--) {
        if (remaining == K[i - 1][w]) {
            continue;
        }
        else {
            items.push_back(i - 1);
            remaining = remaining - values[i - 1];
            w = w - weights[i - 1];
        }
    }
    return items;
}

void print_results(const std::vector<int>& results) 
{
    std::cout << results.size() << std::endl;
    for (auto index: results) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}



int main()
{
    std::ios_base::sync_with_stdio(false);

    //auto& cin = std::cin;
    cin.tie(nullptr);

    bool new_test_case = true;
    while (auto testCase = process_input()) {
        auto& testCaseVal = *testCase;
        auto test_case_results = knapsack(testCaseVal.C, testCaseVal.values, testCaseVal.weights);
        print_results(test_case_results);
    }
}