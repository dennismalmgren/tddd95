
#include <bits/stdc++.h>
std::ifstream cin("in_perfectpowers.txt");
//auto& cin = std::cin;


struct TestCase {
    TestCase() {
        
    }
    std::vector<int> numbers;
};

TestCase process_input() 
{
    int n;
    TestCase testCase;
    while (true) {
        cin >> n;
        if (n == 0) {
            break;
        }
        testCase.numbers.push_back(n);
    }
    return testCase;
}

int find_p(long number) 
{
    long double epsilon = 1e-6;
    int sign = number / std::abs(number);
    number = std::abs(number);
    for (long p_max = 31; p_max >= 1; p_max--) {
        if (p_max % 2 == 0 && sign < 0) {
            continue;
        }
        long double x_test = std::pow((long double)number, (long double)1.0/(long double)p_max);
        if (std::abs(x_test - std::floor(x_test)) < epsilon) {
            long x_test_1 = std::floor(x_test);
            if (std::abs(std::pow(x_test_1, p_max) - number) < epsilon) {
                return p_max;
            }
        }
        
        if (std::abs(x_test - std::ceil(x_test)) < epsilon) {
            long x_test_2 = std::ceil(x_test);
            if (std::abs(std::pow(x_test_2, p_max) - number) < epsilon) {
                return p_max;
            }   
        }        
    }
    return 1;
}

void print_result(int result) 
{
    std::cout << result << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    for (auto number: testCase.numbers) {
        auto p = find_p(number);
        print_result(p);
    }
  
    std::cout.flush();
}
