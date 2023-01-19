#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <ios>
#include <limits>

std::ifstream cin("in_longincsubseq.txt");
//auto& cin = std::cin;
using Vec = std::vector<long long>;

struct TestCase {
    TestCase(int n) : n(n), sequence(n){
        
    }
    int n;
    Vec sequence;
};

std::optional<TestCase> process_input() 
{
    int n;
    long long number;
    
    if (cin >> n) {
        TestCase testCase(n);
        for (int i = 0; i < n; i++) {
            cin >> number;
            testCase.sequence[i] = number;
        }
        return testCase;
    }
    return std::nullopt;
}

void print_results(const Vec& results) 
{
    std::cout << results.size() << std::endl;
    for (auto index: results) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
}

auto lis(const Vec& sequence) 
{
    auto n = sequence.size();
    auto max_val = std::numeric_limits<long long>::max();
    Vec ls(n + 1, max_val);
    std::vector<Vec::size_type> ls_ind(n + 1, n + 1);
    std::vector<Vec::size_type> a(n + 1, n + 1);

    ls[0] = std::numeric_limits<long long>::min();
    int longest_subsequence_so_far_index = 0;
    for (auto i = 0; i < n; i++) {
        auto upper_cmp = std::next(ls.begin(), i + 1);
        auto it = std::upper_bound(ls.begin(), std::next(ls.begin(), i + 1), sequence[i]);
        auto prev_it = std::prev(it);
        Vec::size_type index = std::distance(ls.begin(), it);
        Vec::size_type prev_index = std::distance(ls.begin(), prev_it);
        if (*prev_it < sequence[i] && sequence[i] < *it) {
            a[i] = ls_ind[prev_index];
            *it = sequence[i];
            ls_ind[index] = i;
        }
    }

    auto items = Vec();
    auto longest_itr = std::prev(std::find_if(ls.rbegin(), ls.rend(),
                            [&](const auto& e) { return e < std::numeric_limits<long long>::max(); } ).base());
    Vec::size_type index = std::distance(ls.begin(), longest_itr); // 
    index = ls_ind[index];
    while (index != (n+1)) {
        items.push_back(index);
        index = a[index];
    }
    std::reverse(items.begin(), items.end());
    return items;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    bool new_test_case = true;
    while (auto testCase = process_input()) {
        auto& testCaseVal = *testCase;
        auto test_case_results = lis(testCaseVal.sequence);
        print_results(test_case_results);
    }
}
