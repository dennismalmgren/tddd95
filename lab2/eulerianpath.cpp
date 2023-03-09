//distance/parent[] shortest_path(graph G, node start)
//#include "algo_lib.h"
#include <bits/stdc++.h>

std::ifstream cin("in_eulerianpath.txt");
//auto& cin = std::cin;

//using namespace algo;


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