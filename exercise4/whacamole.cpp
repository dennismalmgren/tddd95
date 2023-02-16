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

int whac(int n, int d, std::vector<int>& x, std::vector<int>& y, std::vector<int>& t) {
    // vector of t, x, y
    std::vector<std::vector<std::vector<int>>> state(11, 
            std::vector<std::vector<int>>(n,
            std::vector<int>(n)));
    std::map<int, std::vector<int>> t_x;
    std::map<int, std::vector<int>> t_y;

    for (int i = 0; i < t.size(); i++) {
        if (t_x.find(t[i]) == t_x.end()) {
            t_x[t[i]] = std::vector<int>();
        }
        if (t_y.find(t[i]) == t_y.end()) {
            t_y[t[i]] = std::vector<int>();
        }
        
        t_x[t[i]].push_back(x[i]);
        t_y[t[i]].push_back(t[i]);
    }
    for (int t_eval = 10; t_eval >= 1; t_eval--) {
        auto x_vals = t_x[t_eval];
        auto y_vals = t_y[t_eval];
        for (int i = 0; i < x_vals.size(); i++) {
            state[t][x_vals[i]][y_vals[i]] =
        }
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < n; y++) {

                int left_score = 0; 
                int right_score = 0;
                int up_score = 0;
                int down_score = 0;

            }
        }
    }
    return 0;
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