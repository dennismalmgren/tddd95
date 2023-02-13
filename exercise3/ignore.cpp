#include <bits/stdc++.h>
std::ifstream cin("in_ignore.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase() {
        
    }
    long long k;
};

TestCase process_input() 
{
    TestCase testCase;
    cin >> testCase.k;

    return testCase;
}

long long find_number(long long number) 
{
    long long converted = 0;
    long long base = 7;
    long long exponent = 0;
    while (number > 0) {
        long long digit = number % base;
        converted += digit * std::pow(10, exponent);
        number /= base;
        exponent++;
    }
    long long result = 0;
    std::map<long long, long long> dict;
    dict[0] = 0;
    dict[1] = 1;
    dict[2] = 2;
    dict[3] = 5;
    dict[4] = 9;
    dict[5] = 8;
    dict[6] = 6;
    exponent = 0;
    while (converted > 0) {
        long long digit = dict[converted % 10];
        result += digit * std::pow(10, exponent);
        converted /= 10;
        exponent++;
    }
    return result;
}

void print_result(long long result) 
{
    std::stringstream num;
    num << result;
    std::string num_str = num.str();
    std::reverse(num_str.begin(), num_str.end());
    std::cout << num_str;
    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int old_k = -1;
    for (int i = 0; i < 1100; i++) {
        auto testCase = process_input();
        if (testCase.k == old_k) {
            break;
        }
        else {
            old_k = testCase.k;
        }
        auto result = find_number(testCase.k);
        print_result(result);
    }
    
}