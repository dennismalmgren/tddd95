// longincsubseq.cpp
// Author: Dennis Malmgren
// Solves the longest increasing subsequence problem.
// Given a list of numbers, find the longest
// subsequence that is strictly increasing.

#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_longincsubseq.txt");
//auto& cin = std::cin;
using Vec = std::vector<long long>;

/// @brief TestCase class
/// Gathers the necessary input to define a longest increasing subsequence problem
/// Has variables for the number n that defines the length of the sequence
/// as well as a vector with the sequence itself.
struct TestCase {
    TestCase(int n) : n(n), sequence(n){
        
    }
    int n;
    Vec sequence;
};

/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin,
/// or nullopt if input is empty.
std::optional<TestCase> process_input() 
{
    int n;
    long long number;
    
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

/// @brief Helper function to print the results. 
/// @param results a list of indices to the longest increasing subsequence
void print_results(const Vec& results) 
{
    std::cout << results.size() << std::endl;
    for (auto index: results) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}

/// @brief Longest increasing subsequence method.
/// Calculates the indices for the longest increasing subsequence
/// Time complexity O(n log n) where n is the length of the sequence of available items.
/// @return the indices to the integers in the longest increasing subsequence
Vec lis(const Vec& sequence) 
{
    auto n = sequence.size();
    auto max_val = std::numeric_limits<long long>::max();
    // keep track of candidates
    // which is the lowest value at which a sequence with this length can be obtained
    // (so far)
    Vec ls(n + 1, max_val);
    std::vector<Vec::size_type> ls_ind(n + 1, n + 1);
    std::vector<Vec::size_type> a(n + 1, n + 1);

    ls[0] = std::numeric_limits<long long>::min();

    for (auto i = 0; i < n; i++) {
        auto upper_cmp = std::next(ls.begin(), i + 1);
        auto it = std::upper_bound(ls.begin(), std::next(ls.begin(), i + 1), sequence[i]);
        auto prev_it = std::prev(it);
        Vec::size_type index = std::distance(ls.begin(), it);
        Vec::size_type prev_index = std::distance(ls.begin(), prev_it);
        if (*prev_it < sequence[i] && sequence[i] < *it) {
            a[i] = ls_ind[prev_index];
            *it = sequence[i];
            ls_ind[index] = i;
        }
    }

    auto items = Vec();
    // Extract the actual candidate 
    auto longest_itr = std::prev(std::find_if(ls.rbegin(), ls.rend(),
                            [&](const auto& e) { return e < std::numeric_limits<long long>::max(); } ).base());
    Vec::size_type index = std::distance(ls.begin(), longest_itr); // 
    index = ls_ind[index];
    while (index != (n+1)) {
        items.push_back(index);
        index = a[index];
    }
    std::reverse(items.begin(), items.end());
    return items;
}

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    bool new_test_case = true;
    while (auto testCase = process_input()) {
        auto& testCaseVal = *testCase;
        auto test_case_results = lis(testCaseVal.sequence);
        print_results(test_case_results);
    }
}
