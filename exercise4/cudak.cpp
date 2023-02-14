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
    auto delta = b.size() - a.size();
    for (int i = 0; i < delta; i++) {
        a = "0" + a;
    }
    
    std::vector<std::vector<long long>> state(b.size() + 1, std::vector<long long>(s + 1));
    std::vector<std::vector<int>> predecessor_state(b.size() + 1, std::vector<int>(s + 1, 10));
    std::vector<std::vector<int>> predecessor_b(b.size() + 1, std::vector<int>(s + 1, 10));
    std::vector<std::vector<int>> predecessor_a(a.size() + 1, std::vector<int>(s + 1, -1));
    std::vector<std::vector<long long>> state_within_b(b.size() + 1, std::vector<long long>(s + 1));
    std::vector<std::vector<long long>> state_within_a(b.size() + 1, std::vector<long long>(s + 1));
    // need to expand with islower/ishigher
    // fill in pos 0.
    int remove_a = 0;
    int a_sum = 0;
    for (int i = 0; i < a.size(); i++) {
        a_sum += std::stoi(a.substr(i, 1));
    }
    if (a_sum == s) {
        remove_a = 1;
    }
    for (int digit = 0; digit <= 9; digit++) {
        if (digit > s) {
            break;
        }
        state[0][digit] = 1;
        predecessor_state[0][digit] = digit;
        int b_digit = std::stoi(b.substr(b.size() - 1, 1));
        int a_digit = std::stoi(a.substr(a.size() - 1, 1));

        if (digit <= a_digit) {
            state_within_a[0][digit] = 1;
            predecessor_a[0][digit] = digit;
        }
        if (digit <= b_digit) {
            state_within_b[0][digit] = 1;
            predecessor_b[0][digit] = digit;
        }
    }

    // start by assuming only 99999 numbers can be used.
    for (int pos = 0; pos < b.size() - 1; pos++) {
        for (int sum = 0; sum <= s; sum++) {
            if (state[pos][sum] == 0) {
                continue;
            }
            int new_pos = pos + 1;
            int b_digit_pos = b.size() - 1 - new_pos;
            int a_digit_pos = a.size() - 1 - new_pos;
            for (int digit = 0; digit <= 9; digit++) {
                int new_sum = sum + digit;
                if (new_sum <= s) {
                    state[new_pos][new_sum] += state[pos][sum];
                    predecessor_state[new_pos][new_sum] = std::min(predecessor_state[new_pos][new_sum], digit);

                    int b_digit = std::stoi(b.substr(b_digit_pos, 1));
                    if (pos < a.size() - 1) {
                        int a_digit = std::stoi(a.substr(a_digit_pos, 1));

                        if (digit < a_digit) {
                            state_within_a[new_pos][new_sum] += state[pos][sum];
                            predecessor_a[new_pos][new_sum] = std::max(digit, predecessor_a[new_pos][new_sum]);

                        }
                        else if (digit == a_digit)  {
                            state_within_a[new_pos][new_sum] += state_within_a[pos][sum];
                            predecessor_a[new_pos][new_sum] = std::max(digit, predecessor_a[new_pos][new_sum]);
                        }
                    }
                    if (digit < b_digit) {
                        state_within_b[new_pos][new_sum] += state[pos][sum];
                        predecessor_b[new_pos][new_sum] = std::min(digit, predecessor_b[new_pos][new_sum]);
                    }
                    else if (digit == b_digit) {
                        state_within_b[new_pos][new_sum] += state_within_b[pos][sum];
                        predecessor_b[new_pos][new_sum] = std::min(digit, predecessor_b[new_pos][new_sum]);
                    }

                }
            }
        }
    }

    // Find the result.
    // First number is the number of integers
    // second number is the lowest integer.
    // we can find the lowest integer by 
    // a) finding the first position in which it is achieved and
    // b) sorting the numbers from lowest to largest in their positions.
    std::pair<long long, long long> result;
    result.first = state_within_b[state_within_b.size() - 2][s] - state_within_a[state_within_a.size() - 2][s] + remove_a;
    // now we need to find the minimum solution.
    if (remove_a) {
        result.second = a_num;
    }
    else {
        std::string digits;
        for (int pos = 0; pos < state.size() - 1; pos++) {
            if (state[pos][s] > 0) {
                // solution is extracted starting at pos.
                int backtrack_s = s;
                for (int digit_pos = pos; digit_pos >= 0; digit_pos--) {
                    int digit = predecessor_state[digit_pos][backtrack_s];
                    digits += std::to_string(digit);
                    backtrack_s -= digit;
                }
                result.second = std::stoi(digits);
                break;
            }
        }

    }
    // long long alternate_result = state_within_b[state_within_b.size() - 2][s] - state_within_a[state_within_a.size() - 2][s] + remove_a;
    // for (int pos = 0; pos < b.size(); pos++) {
    //     result.first += state_within_b[pos][s];
    //     if (pos < a.size()) {
    //         result.first -= state_within_a[pos][s];
    //     }
    // }
    // result.first += remove_a;
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