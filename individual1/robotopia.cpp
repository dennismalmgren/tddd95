#include <bits/stdc++.h>
std::ifstream cin("in_robotopia.txt");
//auto& cin = std::cin;

using ll = long long;

struct TestCase {
    TestCase(int n): A(n, std::vector<ll>(n)), b(n) {
        
    }

    std::vector<std::vector<ll>> A;
    std::vector<ll> b;
};

int process_header() 
{
    int n;
    cin >> n;
    return n;
}

TestCase process_input() 
{
    ll a1, l1, a2, l2, a_t, l_t;
    cin >> l1 >> a1 >> l2 >> a2 >> l_t >> a_t;

    TestCase testCase(2);
    testCase.A[0][0] = l1;
    testCase.A[0][1] = l2;
    testCase.A[1][0] = a1;
    testCase.A[1][1] = a2;
    testCase.b[0] = l_t;
    testCase.b[1] = a_t;
    
    return testCase;
}

struct TestCaseResult {
    bool valid;
    std::pair<ll, ll> result;
};


void print_result(int result) 
{
    std::cout << result << "\n";
}

long long gcd(long long a, long long b) 
{
    return b == 0 ? a : gcd(b, a % b);
}

long long lcm(long long a, long long b) 
{
    return a / gcd(a, b) * b;
}

TestCaseResult linear_solve(std::vector<std::vector<ll>>& A, std::vector<ll>& b) 
{
    TestCaseResult result;
    result.valid = true;
    bool found_one = false;
    for (ll i = 1; i < 10000; i++) {
        auto type_1 = i;
        auto legs = b[0] - type_1 * A[0][0];
        auto arms = b[1] - type_1 * A[1][0];
        auto legs_factor = legs / A[0][1];
        auto arms_factor = arms / A[1][1];

        if (legs_factor == arms_factor &&
            legs_factor * A[0][1] == legs &&
            arms_factor * A[1][1] == arms &&
            legs > 0 &&
            arms > 0
            ) {
                if (!found_one) {
                    found_one = true;
                    result.valid = true;
                    result.result = std::make_pair(type_1, legs_factor);        
                }
                else {
                    result.valid = false;
                    return result;
                }                
            }
    }
    if (!found_one) {
        result.valid = false;
        return result;
    }
    else {
        result.valid = true;
        return result;
    }
   
}

void print_result(const TestCaseResult& testCaseResult) 
{ 
    if (!testCaseResult.valid) {
        std::cout << "?\n";
    }
    else {
        std::cout << testCaseResult.result.first << " " << testCaseResult.result.second << "\n";
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n = process_header();
    for (int i = 0; i < n; i++) {
        auto testCase = process_input();
        auto result = linear_solve(testCase.A, testCase.b);
        print_result(result);
    }


    std::cout.flush();
}