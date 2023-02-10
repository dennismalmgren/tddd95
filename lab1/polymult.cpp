
#include <bits/stdc++.h>
std::ifstream cin("in_polymult.txt");
//auto& cin = std::cin;

const double PI = std::acos(-1);

struct TestCase {
    TestCase() {
        
    }
    std::vector<int> poly1;
    std::vector<int> poly2;
};

int process_header() {
    int t;
    cin >> t;
    return t;
}

TestCase process_input() 
{
    int n, a;
    TestCase testCase;
    cin >> n;
    testCase.poly1.reserve(n + 1);
    for (int i = 0; i <= n; i++) {
        cin >> a;
        testCase.poly1.push_back(a);
    }
    cin >> n;
    testCase.poly2.reserve(n + 1);
    for (int i = 0; i <= n; i++) {
        cin >> a;
        testCase.poly2.push_back(a);
    }
    return testCase;
}

void inverse_fft(std::vector<std::complex<double>>& poly) 
{
    int n = poly.size();
    if (n == 1) {
        return;
    }

    std::vector<std::complex<double>> even(n / 2);
    std::vector<std::complex<double>> odd(n / 2);
    for (int i = 0; i < n / 2; i++) {
        even[i] = poly[2 * i];
        odd[i] = poly[2 * i + 1];
    }

    inverse_fft(even);
    inverse_fft(odd);

    double angle_step = -2 * PI / n;
    std::complex<double> w(1);
    std::complex<double> step_w(std::cos(angle_step), std::sin(angle_step));
    std::complex<double> one_half(1/2);

    for (int i = 0; i < n / 2; i++) {
        auto w_fact = w * odd[i];
        auto even_term = even[i] + w_fact;
        auto odd_term  = even[i] - w_fact;
        odd_term /= 2;
        even_term /= 2;
        poly[i] = even_term;
        poly[i  + n / 2] = odd_term;
        w *= step_w;
    }
}

void fft(std::vector<std::complex<double>>& poly) 
{
    int n = poly.size();
    if (n == 1) {
        return;
    }

    std::vector<std::complex<double>> even(n / 2);
    std::vector<std::complex<double>> odd(n / 2);
    for (int i = 0; i < n / 2; i++) {
        even[i] = poly[2 * i];
        odd[i] = poly[2 * i + 1];
    }

    fft(even);
    fft(odd);

    double angle_step = 2 * PI / n;
    std::complex<double> w(1);
    std::complex<double> step_w(std::cos(angle_step), std::sin(angle_step));
    for (int i = 0; i < n / 2; i++) {
        poly[i] = even[i] + w * odd[i];
        poly[i  + n / 2] = even[i] - w * odd[i];
        w *= step_w;
    }

}

void poly_mult(std::vector<int>& poly1,
        std::vector<int>& poly2,
        std::vector<int>& result) 
{
    int x1 = std::ceil(std::log(poly1.size() + poly2.size()) / std::log(2));

    std::vector<std::complex<double>> poly1cd(poly1.begin(), poly1.end());
    std::vector<std::complex<double>> poly2cd(poly2.begin(), poly2.end());
    poly1cd.resize(std::pow(2, x1));
    poly2cd.resize(std::pow(2, x1));

    fft(poly1cd);
    fft(poly2cd);
    std::transform(poly1cd.begin(), 
                    poly1cd.end(), 
                    poly2cd.begin(), 
                    poly1cd.begin(),
                std::multiplies<std::complex<double>>());
    inverse_fft(poly1cd);
    result.resize(poly1cd.size());
    std::transform(poly1cd.begin(), 
                    poly1cd.end(),
                    result.begin(),
                    [](std::complex<double>& elem) { return static_cast<int>(std::round(elem.real()));});
    auto first_non_zero = std::find_if(result.rbegin(), result.rend(), [](int val) {return val != 0;});
    result.erase(first_non_zero.base(), result.end());
}

void print_result(const std::vector<int>& result) 
{
    std::cout << (result.size() - 1) << "\n";
    for (auto val: result) {

        std::cout << val << " ";
    }
    std::cout << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = process_header();
    for (int i = 0; i < t; i++) {
        auto testCase = process_input();
        std::vector<int> result;
        poly_mult(testCase.poly1, testCase.poly2, result);
        print_result(result);
    }
  
    std::cout.flush();
}
