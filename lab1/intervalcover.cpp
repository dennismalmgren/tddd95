// intervalcover.cpp
// Author: Dennis Malmgren
// Solves the interval covering problem.
// Given a numeric interval [A, B]
// and a set of candidate intervals {(a_i, b_i)},
// find a minimal subset that covers the numeric interval.


#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_intervalcover.txt");
//auto& cin = std::cin;

/// @brief 
/// Helper function obtained from
/// https://gist.github.com/HViktorTsoi/58eabb4f7c5a303ced400bcfa816f6f5
/// creates an array of indices to the argument array 
/// and sorts those indices based on the values in array
/// @param array array of elements that are used as comparing elements in a sorting operation
/// @return a sorted vector of indices
template<typename T>
std::vector<size_t> argsort(const std::vector<T> &array) {
    std::vector<size_t> indices(array.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [&array](int left, int right) -> bool {
                  // sort indices according to corresponding array element
                  return array[left] < array[right];
              });

    return indices;
}


/// @brief TestCase class
/// Gathers the necessary input to define an interval covering problem
/// Has variables for the numeric interval (a, b), the number of
/// intervals that can be used for covering, n
/// as well as a list of (a_i, b_i) pairs.
struct TestCase {
    /// @brief Constructor
    /// @param a leftmost interval point to cover
    /// @param b rightmost interval point to cover
    /// @param n the number of intervals that can be used for covering
    TestCase(double a, double b, int n) : a(a), b(b), n(n), intervals(n) {
        
    }
    double a;
    double b;
    int n;
    std::vector<std::pair<double, double>> intervals;
};


/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin,
/// or nullopt if input is empty.
std::optional<TestCase> process_input() 
{
    double a, b;
    int n = 0;
    double a_i, b_i;

    cin >> a >> b >> n;
    if (n == 0) {
        return std::nullopt;
    }
    TestCase testCase(a, b, n);

    for (int i =0; i < n; i++) {
        cin >> a_i >> b_i;
        testCase.intervals[i] = std::make_pair(a_i, b_i);
    }

    return testCase;
}

/// @brief cover method, finds a minimal subset of intervals to cover the interval [a, b].
/// Uses O(n log n) time and O(n) memory.
/// @param a leftmost interval point to cover
/// @param b rightmost interval point to cover
/// @param intervals available (a_i, b_i) pairs that define intervals that can be used for covering.
/// @return indices in intervals that could be used to cover the [a, b] interval, or an empty vector if
// the problem is impossible.
std::vector<int> cover(double a, double b, std::vector<std::pair<double, double>>& intervals)
{
    const double epsilon = 1e-9;
    std::vector<int> result;

    // handle special case
    if (std::abs(a - b) < epsilon) {
        for (int i = 0; i < intervals.size(); i++) {
            auto interval = intervals[i];
            if (interval.first <= a && interval.second >= a)  {
                result.push_back(i);
                return result;
            }
        }
    }

    std::vector<double> a_vec;
    for (auto interval: intervals) {
        a_vec.push_back(interval.first);
    }
    auto a_indices = argsort(a_vec);
    // t indicates the next point we need to cover.
    auto t = a; 
    int candidate_index = -1;
    // Iterate through the intervals from left to right.
    // Include the interval with the right-most endpoint 
    // that stills maintains coverage across the interval
    // only begin including intervals when they actually cover the area we are interested in.
    for (auto index: a_indices) {
        auto cmp_interval = intervals[index];

        if (cmp_interval.second <= t) { // interval does not cover our area
            continue;
        }
        if (cmp_interval.first > b) { // interval starts on the right side of the area we want to cover
            break;
        }
        if (cmp_interval.first <= t && cmp_interval.second > t) { 
            // this is a covering interval,
            // we want to keep the best one.
            if (candidate_index == -1) {
                candidate_index = index;
            }
            else if (cmp_interval.second > intervals[candidate_index].second) {
                candidate_index = index;
            }
        }
        else {
            // We've gone past t. 
            // Do we have a candidate?
            // Are we done or do we need more candidates? 
            if (candidate_index != -1) {
                result.push_back(candidate_index);
                t = intervals[candidate_index].second;
                candidate_index = -1;
                if (t >= b) {
                    break;
                }
                // we just moved t, so check again.
                if (cmp_interval.first <= t && cmp_interval.second > t) {
                    candidate_index = index;
                }
                else {
                    continue;
                }
            }
            else {
                // no candidate.
                break;
            }
        }
    }

    if (candidate_index != -1) {
        // We still have an active candidate
        // but ran out of new intervals that could be better so lets
        // keep this one
        result.push_back(candidate_index);
        t = intervals[candidate_index].second;
    }

    // We did the best we could, is it enough?
    if (t >= b) {
        return result;
    }

    return std::vector<int>();
}

/// @brief Helper function to print the results. 
/// If results is empty, 'impossible' is written, otherwise the size
/// and contents are written.
/// @param results a list of indices, or empty.
void print_results(const std::vector<int>& results) 
{
    if (results.size() == 0) {
        std::cout << "impossible" << "\n";
    }
    else {
        std::cout << results.size() << "\n";
        for (auto index: results) {
            std::cout << index << " ";
        }
        std::cout << "\n";
    }
}

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// Assumes at most 30 test cases in the input. 
/// @return 0
int main()
{
    // Speed up I/O
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int i = 0; i < 30; i++) {
        auto testCaseOpt = process_input();
        if (testCaseOpt.has_value()) {
            auto testCase = testCaseOpt.value();
            auto result = cover(testCase.a, testCase.b, testCase.intervals);
            print_results(result);
        }
        else {
            break;
        }
    }

    // Speed up I/O
    std::cout.flush();
}