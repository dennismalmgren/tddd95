#include <bits/stdc++.h>
std::ifstream cin("in_cudak.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase() {
        
    }
    std::string A;
    long long a;
    std::string B;
    long long b;
    int S;
};

TestCase process_input() 
{
    TestCase testCase;
    cin >> testCase.A >> testCase.B >> testCase.S;
    std::stringstream lower(testCase.A);
    std::stringstream upper(testCase.B);
    lower >> testCase.a;
    upper >> testCase.b;
    return testCase;
}

std::pair<long long, long long> find_values(std::string a, std::string b, 
                                long long a_num, long long b_num, int s) 
{
  
    auto max_pos = b.size();
    std::pair<long long, long long> result;
    
    std::vector<std::vector<long long>> state(20, std::vector<long long>(s + 1));
    // need to expand with islower/ishigher
    // fill in pos 0.
    for (int sum = 0; sum <= 9; sum++) {
        state[0][sum] = 1;
    }

    // start by assuming only 99999 numbers can be used.
    for (int pos = 0; pos < b.size() - 1; pos++) {
        for (int sum = 0; sum <= s; sum++) {
            if (state[pos][sum] == 0) {
                continue;
            }
            for (int digit = 0; digit <= 9; digit++) {
                int new_sum = sum + digit;
                if (new_sum <= s) {
                    state[pos + 1][new_sum] += state[pos][sum] + 1;
                }
            }
        }
    }
    return result;
}

void print_result(std::pair<long long, long long> result) 
{
    std::cout << result.first << "\n" << result.second << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    auto result = find_values(testCase.A, testCase.B, 
                            testCase.a, testCase.b, testCase.S);
    print_result(result);
    std::cout.flush();
}