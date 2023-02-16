// union_find.cpp
// Author: Dennis Malmgren
// Implements the disjoint dataset datastructure
// and performs basic operations (join and query)
// according to a command list
#include <bits/stdc++.h>


// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_union_find.txt");
//auto& cin = std::cin;
using Vec = std::vector<int>;

/// @brief Defines supported operations
enum class OperatorType {
    join,
    query
};

/// @brief Defines potential results of an operation
/// no/yes are reserved for query operations (are elements
/// a and b in the same set?)
/// nothing is the expected result for a join operation.
enum class OperatorResult {
    no,
    yes,
    nothing
};

/// @brief TestCase class
/// Gathers the necessary input to define a set of union-find operations
/// Has variables for Q, the number of operations, and N, the number of 
/// elements in the disjoint set
///  and a list of operations together with their first and second operands.
struct TestCase {
    TestCase(int n, int q) : N(n), Q(q), l_vec(q), r_vec(q), operations(q) {
        
    }
    int Q;
    int N;

    Vec l_vec;
    Vec r_vec;
    std::vector<OperatorType> operations;
};

/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin,
/// or nullopt if input is empty.
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

/// @brief Helper function to print the results of a single operation. 
/// @param result the enum result to translate to output. Ignores nothing-results.
void print_result(const OperatorResult result) 
{
    if (result == OperatorResult::no) {
        std::cout << "no\n";
    }
    else if (result == OperatorResult::yes) {
        std::cout << "yes\n";
    }
}


/// @brief Helper class to implement disjoint sets.
/// supports union and find operations.
class DisjointSet {
    public:
        /// @brief Constructor. Creates an initial structure
        /// with all elements in individual sets.
        /// @param n the number of elements in the set.
        DisjointSet(int n) : n(n), parent(n), rank(n) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        
        /// @brief Finds the root of the set for an element.
        /// also performs path compression during the find, as a side-effect.
        /// Executes in O(log n)
        /// @param elem the element to find the root for.
        /// @return the root of the element. 
        int find_set(int elem) {
            if (elem == parent[elem]) {
                return elem;
            }
            return parent[elem] = find_set(parent[elem]);
        }

        /// @brief Merges the sets of two elements.
        /// uses the rank of the individual sets to optimize the depth
        /// of the resulting tree structure.
        /// Executes in O(log n)
        /// @param elem1 Element one.
        /// @param elem2  Element two.
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
        std::vector<int> parent;
        std::vector<int> rank;
};


/// @brief Performs a single operation on a disjoint set.
/// @param set the set to perform 
/// @param oper the operation to perform (join or query)
/// @param lhs first operand
/// @param rhs second operand
/// @return the result of the operation.
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

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    DisjointSet set(testCase.N);

    for (int i = 0; i < testCase.Q; i++) {
        auto result = union_or_find(set, testCase.operations[i], testCase.l_vec[i], testCase.r_vec[i]);
        print_result(result);
    }
    std::cout.flush();
    return 0;
}
