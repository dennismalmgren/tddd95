// union_find.cpp
// Author: Dennis Malmgren
// Implements the disjoint dataset datastructure
// and performs basic operations (join and query)
// according to a command list
#include <bits/stdc++.h>


// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_numberseteasy.txt");
//auto& cin = std::cin;
using Vec = std::vector<int>;

template<typename T>
std::vector<size_t> argsort(const std::vector<T> &array) {
    std::vector<size_t> indices(array.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [&array](int left, int right) -> bool {
                  // sort indices according to corresponding array element
                  return array[left] > array[right];
              });

    return indices;
}

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
    TestCase(int c) : C(c), A(c), B(c), P(c) {
        
    }
    int C;

    Vec A;
    Vec B;
    Vec P;
};

/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin,
/// or nullopt if input is empty.
TestCase process_input() 
{
    int c;
    cin >> c;
    TestCase testCase(c);
    int a, b, p;

    for (auto i = 0; i < c; i++) {
        cin >> a >> b >> p;
        testCase.A[i] = a;
        testCase.B[i] = b;
        testCase.P[i] = p;
    }
    return testCase;
}

/// @brief Helper function to print the results of a single operation. 
/// @param result the enum result to translate to output. Ignores nothing-results.
void print_result(int testCaseIndex, int number_of_sets)
{
    std::cout << "Case #" << testCaseIndex << ": " << number_of_sets << "\n";
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
            return find_set(parent[elem]);
//            return parent[elem] = find_set(parent[elem]);
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

std::vector<int> generate_primes(int top) {
    std::vector<int> result;
    result.push_back(2);
    for (int i = 3; i < top; i++) {
        bool found_factor = false;
        for (int value: result) {
            if (i % value == 0) {
                found_factor = true;
                break;
            }
        }
        if (!found_factor) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> primes(int top, int bottom) {
    std::vector<int> result;
    result.push_back(2);
    for (int i = 3; i < top; i++) {
        bool found_factor = false;
        for (int value: result) {
            if (i % value == 0) {
                found_factor = true;
                break;
            }
        }
        if (!found_factor) {
            result.push_back(i);
        }
    }

    for (int i =0; i < result.size(); i++) {
        if (result[i] >= bottom) {
            if (i > 0) {
                result.erase(result.begin(), result.begin() + i);
            }
            break;
        }
    }
    return result;
}

long long gcd(long long a, long long b) 
{
    return b == 0 ? a : gcd(b, a % b);
}

int count_sets(int a, int b, int p, DisjointSet& the_set,
std::vector<int>& prime_vec) {
    DisjointSet set(1100);
    auto relevant_primes_start = std::find_if(prime_vec.begin(), prime_vec.end(), [&p](int prime) { return prime >= p;});
    auto relevant_primes_end = std::find_if(prime_vec.begin(), prime_vec.end(), [&b](int prime) {return prime > b;});

    for (int i = std::max(a, p); i <= b; i++) {
        for (auto iter = relevant_primes_start; iter != relevant_primes_end; iter++) {

            if (i % (*iter) == 0) {
                set.union_sets(i, *iter);
            }
        }
    }

    // now count.
    std::set<int> set_parents;
    for (int i = a; i <= b; i++) {
        auto set_id = set.find_set(i);
        set_parents.insert(set_id);
    }
    return set_parents.size();
}
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
    DisjointSet set(1000);
    auto testCaseIndices = argsort(testCase.P);
    std::vector<int> results;
    std::vector<int> primes = generate_primes(2000);

    for (int i = 0; i < testCase.C; i++) {
        auto result = count_sets(testCase.A[testCaseIndices[i]], testCase.B[testCaseIndices[i]], testCase.P[testCaseIndices[i]], set, primes);
        results.push_back(result);
    }
    for (int i = 0; i < testCase.C; i++) {
        print_result(i + 1, results[testCaseIndices[i]]);
    }
    std::cout.flush();
    return 0;
}
