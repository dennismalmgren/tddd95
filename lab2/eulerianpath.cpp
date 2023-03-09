// eulerianpath.cpp
// Author: Dennis Malmgren
// Finds the eulerian path in a directed graph
// if there is one, or identifies if there is not.
#include <bits/stdc++.h>

// Helper variables to easily swap between test input (for debuggging) and Kattis.
std::ifstream cin("in_eulerianpath.txt");
//auto& cin = std::cin;

//using namespace algo;

// @brief TestCase class
/// Gathers the necessary input to define a shortest path problem
/// Uses an adjacency list representation of a graph with ingoing edges instead
/// of outgoing.
struct TestCase {
    TestCase(int n, int m) 
    : n(n), m(m),
      ingoing(n),
      out_degrees(n), 
      in_degrees(n) {
        
    }

    int n;
    int m;
    std::vector<std::vector<int>> ingoing;
    std::vector<int> out_degrees;
    std::vector<int> in_degrees;
    bool is_end() 
    {
        return n == 0 && m == 0;
    }
};

/// @brief Reads the graph and pre-calculates vertex degrees.
/// @return A complete specification of a test case.
TestCase process_input() 
{
    int n, m;
    cin >> n >> m;
    TestCase testCase(n, m);
    if (testCase.is_end()) {
        return testCase;
    }

    int u, v, w;
    int odd_count = n;
    for (int i = 0; i < m; i++) {
        cin >> u >> v;
        testCase.out_degrees[u]++;
        testCase.in_degrees[v]++;
        testCase.ingoing[v].push_back(u);
    }

    return testCase;
}

/// @brief Prints the path as traversed vertices, or Impossible if there is
/// no path.
/// @param result A vector of traversed vertices in an Eulerian path order, 
/// or empty if there is no path.
void print_result(std::vector<int>& result) 
{
    if (result.size() == 0) {
        std::cout << "Impossible\n";
    }
    else {
        for (int i = 0; i < result.size(); i++) {
            std::cout << result[i];
            if (i != result.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

/// @brief Solves the Eulerian path by backtracking dfs in O(E) time.
/// @param ingoing Adjacency list graph with ingoing vertices. 
/// @param m The total number of edges.
/// @param in_degrees The in-degree of each vertex.
/// @param out_degrees The out-degree of each vertex.
/// @return A list of vertices that can be traversed in an Eulerian path,
/// or empty if there is no such path.
std::vector<int> solve(std::vector<std::vector<int>>& ingoing, 
                        int m,
                        std::vector<int>& in_degrees, 
                        std::vector<int>& out_degrees) 
{
    std::vector<int> result;
    int n = ingoing.size();
    int starting_point = -1;
    int ending_point = -1;
    bool fail = false;
    for (int i = 0; i < n; i++) {
        if (in_degrees[i] + 1 == out_degrees[i]) {
            if (starting_point != -1) {
                fail = true;
                break;
            }
            else {
                starting_point = i;
            }
        }
        else if (in_degrees[i] == out_degrees[i] + 1) {
            if (ending_point != -1) {
                fail = true;
                break;
            }
            else {
                ending_point = i;
            }
        }
        else if (in_degrees[i] != out_degrees[i]) {
            fail = true;
            break;
        }
    }

    if (fail) {
        return result;
    }

    if (starting_point == -1) {
        starting_point = 0;
    }

    std::stack<int> stack;
    int start = 0;
    if (starting_point != -1 && ending_point != -1) {
        start = ending_point;
    }
    stack.push(start);
    while (!stack.empty()) {
        int current = stack.top();
        if (ingoing[current].size() == 0) {
            result.push_back(current);
            stack.pop();
            continue;
        }
        else {
            stack.push(ingoing[current].back());
            ingoing[current].pop_back();
        }
    }

    if (result.size() != m + 1) {
        result.clear();
        return result;
    }

    return result;
}

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true) {
        auto testCase = process_input();
        if (testCase.is_end()) {
            break;
        }
        auto result = solve(testCase.ingoing, testCase.m, testCase.in_degrees, testCase.out_degrees);
        print_result(result);
    }

    std::cout.flush();
}