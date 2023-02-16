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
    // take care of a == b.
    std::pair<long long, long long> result;
    if (a_num == b_num) {
        result.first = 1;
        result.second = s;
        return result;
    }

    auto delta_size = b.size() - a.size();
    for (int i = 0; i < delta_size; i++) { // pad with zeroes.
        a = "0" + a;
    }
    // state index: pos, is_valid_b, is_valid_a, sum. store count.
    std::vector<std::vector<std::vector<std::vector<long long>>>> state(b.size(),
        std::vector<std::vector<std::vector<long long>>>(2,
        std::vector<std::vector<long long>>(2,
        std::vector<long long>(s + 1))));

    // state index: pos, is_valid_b, is_valid_a, sum. store minimum prev digit.
    std::vector<std::vector<std::vector<std::vector<int>>>> pred(b.size(),
        std::vector<std::vector<std::vector<int>>>(2,
        std::vector<std::vector<int>>(2,
        std::vector<int>(s + 1, 10))));
        
    std::vector<int> b_digits;
    std::vector<int> a_digits;
    for (int i = 0; i < b.size(); i++) {
        b_digits.push_back(std::stoi(b.substr(b.size() - 1 - i, 1)));
    }

    for (int i = 0; i < a.size(); i++) {
        a_digits.push_back(std::stoi(a.substr(a.size() - 1 - i, 1)));
    }
    
    for (int next_num = 0; next_num <= 9; next_num++) {
        if (next_num > s) {
            break;
        }
        int is_valid_b = static_cast<int>(next_num < b_digits[0]);
        int is_valid_a = static_cast<int>(next_num > a_digits[0]);
        state[0][is_valid_b][is_valid_a][next_num] = 1;
        pred[0][is_valid_b][is_valid_a][next_num] = next_num;
    }

    // start by assuming only 99999 numbers can be used.
    for (int pos = 1; pos < b.size(); pos++) {
        for (int next_num = 0; next_num <= 9; next_num++) {
            //int is_valid = static_cast<int>(next_num > a_digits[pos] && next_num < b_digits[pos]);
            int is_valid_b = static_cast<int>(next_num < b_digits[pos]);
            int is_valid_a = static_cast<int>(next_num > a_digits[pos]);
            for (int sum = 0; sum <= s; sum++) {
                int next_sum = sum + next_num;
                if (next_sum <= s) {
                    state[pos][is_valid_b][is_valid_a][next_sum] 
                        += state[pos - 1][0][0][sum] + state[pos - 1][1][1][sum]
                        + state[pos - 1][0][1][sum] + state[pos - 1][1][0][sum];
                    pred[pos][is_valid_b][is_valid_a][next_sum] = std::min(pred[pos][is_valid_b][is_valid_a][next_sum],
                                                                            next_num);
                }
            }
        }
    }



    result.first = 0;
    auto backtrack_sum_b = s;
    auto backtrack_sum_a = s;
    int b_digit_sum = 0;
    int a_digit_sum = 0;

    int diverge_location = 0;
    for (int i = 0; i < b.size(); i++) {
        b_digit_sum += b_digits[i];
        a_digit_sum += a_digits[i];
        if (b_digits[i] > a_digits[i]) {
            diverge_location = std::max(i, diverge_location);
        }
    }
    // now to find the lowest number!
    for (int i = b.size() - 1; i >= 0; i--) {
        auto old_result_first = result.first;

        if (i > diverge_location) {
            if (backtrack_sum_b >= 0) {
                result.first += state[i][1][1][backtrack_sum_b];
                backtrack_sum_a -= b_digits[i];
                backtrack_sum_b -= b_digits[i];
            }
        }
        else if (i == diverge_location) {
            result.first += state[i][1][1][backtrack_sum_b];
            backtrack_sum_b -= b_digits[i];
            backtrack_sum_a -= a_digits[i];
        }
        else {
            // do b-boundary only now.
            if (backtrack_sum_b >= 0) {
                result.first += state[i][1][0][backtrack_sum_b] + state[i][1][1][backtrack_sum_b];
                backtrack_sum_b -= b_digits[i];
            }
            if (backtrack_sum_a >= 0) {
                result.first += state[i][0][1][backtrack_sum_a] + state[i][1][1][backtrack_sum_a];
                backtrack_sum_a -= a_digits[i];
            }
        }
    }

    if (b_digit_sum == s) {
        result.first += 1;
    }
    if (a_digit_sum == s) {
        result.first += 1;
    }

    // now we find the lowest.
    auto delta_sum = s - a_digit_sum;
    long long lowest_value = 0;
    for (int pos = 0; pos < a.size(); pos++) {
        long long capacity = std::min(9 - a_digits[pos], delta_sum);
        delta_sum -= capacity;
        lowest_value += (capacity + a_digits[pos]) * std::pow(10, pos);
    }
    result.second = lowest_value;
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