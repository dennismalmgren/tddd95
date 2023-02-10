#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <ios>
#include <limits>
#include <numeric>
#include <iomanip>
#include <cmath>

std::ifstream cin("in_union_find.txt");
//auto& cin = std::cin;
using Vec = std::vector<int>;

enum class OperatorType {
    join,
    query
};
enum class OperatorResult {
    no,
    yes,
    nothing
};

struct TestCase {
    TestCase(int n, int q) : N(n), Q(q), l_vec(q), r_vec(q), operations(q) {
        
    }
    int Q;
    int N;

    Vec l_vec;
    Vec r_vec;
    std::vector<OperatorType> operations;
};

TestCase process_input() 
{
    int n, q;
    char oper;
    int lval, rval;
    cin >> n >> q;
    TestCase testCase(n, q);
    for (auto i = 0; i < q; i++) {
        cin >> oper >> lval >> rval;
        if (oper == '?') {
            testCase.operations[i] = OperatorType::query;
        }
        else {
            testCase.operations[i] = OperatorType::join;
        }
        testCase.l_vec[i] = lval;
        testCase.r_vec[i] = rval;
    }
    return testCase;
}

void print_result(const OperatorResult result) 
{
    if (result == OperatorResult::no) {
        std::cout << "no\n";
    }
    else if (result == OperatorResult::yes) {
        std::cout << "yes\n";
    }
}

class DisjointSet {
    // Add path compression

    public:
        DisjointSet(int n, bool joinByRank) : n(n), parent(n), rank(n),
        joinByRank(joinByRank) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        
        int find_set(int elem) {
            if (elem == parent[elem]) {
                return elem;
            }
            return parent[elem] = find_set(parent[elem]);
        }

        void union_sets(int elem1, int elem2) {
            auto a = find_set(elem1);
            auto b = find_set(elem2);
            if (a != b) {
                if (rank[a] < rank[b]) {
                    std::swap(a, b);
                }
                parent[b] = a;
                if (rank[a] == rank[b]) {
                    rank[a]++;
                }
            }
        }

    private:
        int n;
        bool joinByRank;
        std::vector<int> parent;
        std::vector<int> rank;
};


OperatorResult union_or_find(DisjointSet& set, OperatorType oper, int lhs, int rhs) {
    if (oper == OperatorType::join) {
        set.union_sets(lhs, rhs);
        return OperatorResult::nothing;
    }
    else if (oper == OperatorType::query) {
        auto set1 = set.find_set(lhs);
        auto set2 = set.find_set(rhs);
        if (set1 == set2) {
            return OperatorResult::yes;
        }
        else {
            return OperatorResult::no;
        }
    }
    return OperatorResult::nothing;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    DisjointSet set(testCase.N, true);

    for (int i = 0; i < testCase.Q; i++) {
        auto result = union_or_find(set, testCase.operations[i], testCase.l_vec[i], testCase.r_vec[i]);
        print_result(result);
    }
    std::cout.flush();
}
