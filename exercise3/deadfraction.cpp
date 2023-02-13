#include <bits/stdc++.h>
std::ifstream cin("in_deadfraction.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase() {
        
    }
    std::vector<std::string> numbers;
};

TestCase process_input() 
{
    TestCase testCase;
    std::string line;
    long long whole;
    long long fractional;
    char dot;
    while (true) {
        cin >> line;
        if (line.size() == 1 && line[0] == '0') {
            break;
        }
        line.erase(line.size() - 3);
        testCase.numbers.push_back(line);
    }

    return testCase;
}

long long gcd(long long a, long long b) 
{
    return b == 0 ? a : gcd(b, a % b);
}

long long lcm(long long a, long long b) 
{
    return a / gcd(a, b) * b;
}

std::pair<long long, long long> find_fraction(std::string number) 
{
    std::pair<long long, long long> result;
    result.second = std::numeric_limits<long long>::max();
    std::stringstream input(number);
    std::string fractional;
    std::string whole;
    char delim = '.';
    std::getline(input, whole, delim);
    std::getline(input, fractional, delim);
    std::stringstream output(fractional);
    long long f;
    output >> f;
    long long ten_size = std::pow(10, fractional.size());
    for (int i = 1; i <= fractional.size(); i++) {
        long long exponent = fractional.size() - i;
        long long power = std::pow(10, exponent);
        long long f_prime = 0;
        if (exponent > 0) {
            std::stringstream fractional_output(fractional.substr(0, exponent));
            fractional_output >> f_prime;    
        }
        long long numerator = f - f_prime;
        long long denominator = ten_size - power;
        long long the_gcd = gcd(numerator, denominator);
        numerator /= the_gcd;
        denominator /= the_gcd;
        if (denominator < result.second) {
            result.first = numerator;
            result.second = denominator;
        }
    }
    // find smallest deminator that explains fractional.
     
    return result;
}

void print_result(std::pair<long long, long long> result) 
{
    std::cout << result.first << "/" << result.second << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    for (auto number: testCase.numbers) {
        auto result = find_fraction(number);
        print_result(result);
    }
    std::cout.flush();
}