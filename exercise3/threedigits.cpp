#include <bits/stdc++.h>
std::ifstream cin("in_threedigits.txt");
//auto& cin = std::cin;


struct TestCase {
    TestCase() {
        
    }
    long long n;
};

TestCase process_input() 
{
    TestCase testCase;
    cin >> testCase.n;

    return testCase;
}

std::vector<int> find_digits(long long number) {
    long long product = 1;
    long long cutoff = 100000000000;
    int five_count = 0;
    int two_count = 0;
    for (long long i = 2; i <= number; i++) {
        product *= i;
        while (product % 10 == 0) {
            product /= 10;
        }
        while (product % 5 == 0) {
            product /= 5;
            five_count++;
        }
        while (product % 2 == 0) {
            product /= 2;
            two_count++;
        }
        product %= cutoff;
    }
    if (two_count > five_count) {
        for (int i = 0; i < two_count - five_count; i++) {
            product *= 2;
            product %= cutoff;
        }
    }
    else {
        for (int i = 0; i < five_count - two_count; i++) {
            product *= 5;
            product %= cutoff;
        }
    }
    std::vector<int> result;
    for (int pos = 2; pos >= 0; pos--) {
        int divisor = std::pow(10, pos);
        if (divisor <= product) {
            result.push_back((product / divisor) % 10);
        }
    }


    return result;
}

void print_result(std::vector<int>& result) 
{
    for (auto number: result) {
        std::cout << number;
    }
    std::cout << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    auto result = find_digits(testCase.n);
    print_result(result);

    std::cout.flush();
}