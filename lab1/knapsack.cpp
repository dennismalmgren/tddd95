// knapsack.cpp
// Author: Dennis Malmgren
// Solves the knapsack problem.
// Given a capacity limited knapsack
// select among a set of items in a way that maximizes
// the value that fits in the knapsack.

#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_knapsack.txt");
//auto& cin = std::cin;

using Vec = std::vector<int>;


/// @brief TestCase class
/// Gathers the necessary input to define a knapsack problem
/// Has variables for the capacity C and vectors
/// for the weights and values of the selectable items.
struct TestCase {
    TestCase(int c, int n) : C(c), weights(n), values(n) {
        
    }
    int C;
    Vec weights;
    Vec values;
};

/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin,
/// or nullopt if input is empty.
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

/// @brief Knapsack method.
/// Calculates the optimal subset of items to fit in the knapsack.
/// Time complexity O(n*capacity) where n is the number of available items.
/// @param capacity the maximum capacity that fits in the knapsack
/// @param values vector of values for the items that can be put in the knapsack
/// @param weights vector of weights for the items that can be put in the knapsack
/// @return the indices to the optimal items to include
Vec knapsack(int capacity, Vec& values, Vec& weights) 
{
    auto n = values.size();
    std::vector<std::vector<int>> K(n + 1, std::vector<int>(capacity + 1));
     
    for (auto i = 0; i <= n; i++) {
        for (auto w = 0; w <= capacity; w++) {
            // If we do not use items, we are done.
            if (i == 0 || w == 0) {
                K[i][w] = 0;
            }
            // If there is still room, we can either include item i,
            // or not.
            else if (weights[i - 1] <= w) {
                K[i][w] = std::max(values[i - 1] + K[i - 1][w - weights[i - 1]], K[i - 1][w]);
            }
            else {
            // if there is no room for more items the value is set.
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

/// @brief Helper function to print the results. 
/// @param results a list of indices to the optimal items to include.
void print_results(const std::vector<int>& results) 
{
    std::cout << results.size() << std::endl;
    for (auto index: results) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}


/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
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
    return 0;
}