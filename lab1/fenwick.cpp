
// fenwick.cpp
// Author: Dennis Malmgren
// Uses a fenwick tree to solve prefix sum calculations.

#include <bits/stdc++.h>


// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_fenwick.txt");
//auto& cin = std::cin;

/// @brief Finds the rightmost active bit in an integer.
/// @tparam T the type of the operand
/// @param s the value to find the rightmost bit in.
/// @return the value with only that bit active.
template<typename T>
inline T LSOne(T s) { 
    return s & (-s);
}

/// @brief Helper class that implements the Fenwick datastructure
/// which is an efficient structure for calculating prefix sums dynamically.
/// @tparam T 
template <class T>
class Fenwick {
    public:
        using f_size_t = typename std::vector<T>::size_type;

        /// @brief Constructor. Creates the sums array. Values are 1-indexed.
        /// @param n the number of elements in the array.
        Fenwick(f_size_t n) : n (n), numbers(n + 1) 
        {
        }

        /// @brief Adds a value to a specific index and updates all prefix sums.
        /// Executes in O(log n) time.
        /// @param index the index to add to.
        /// @param number the value to add.
        void add(f_size_t index, T number) 
        {
            index += 1;
            for (f_size_t ind = index; ind < numbers.size(); ind += LSOne((int)ind)) {
                numbers[ind] += number;
            }
        }

        /// @brief Calculates the prefix sum up to some index.
        /// Executes in O(log n) time.
        /// @param end the index to calculate the sum up to.
        /// @return the prefix sum.
        T sum(f_size_t end) 
        {
            end += 1;
            T result = 0;
            for (f_size_t ind = end - 1; ind > 0; ind -= LSOne(ind)) {
                result += numbers[ind];
            }
            return result;
        }
    private:
        int n;
        int offset;
        std::vector<T> numbers;
};

/// @brief Suppported operations in a test case.
enum class OperatorType {
    increment,
    query
};

/// @brief TestCase class
/// Gathers the necessary input to define a prefix sum problem
/// Has variables for the number of operations Q and the length of the array N.
/// Also maintains vectors for the operations - one for the operation types and
/// two for the operands. For unary operation types the second operand is assumed to 
/// have a dummy value so the operand vectors are of equal length.
struct TestCase {
    TestCase(int n, int q) : N(n), Q(q), operations(q), operand_1(q), operand_2(q) {
        
    }

    int Q;
    int N;

    std::vector<OperatorType> operations;
    std::vector<int> operand_1;
    std::vector<int> operand_2;
};

/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
TestCase process_input() 
{
    int n, q;
    char oper;
    int val_1, val_2;
    cin >> n >> q;
    TestCase testCase(n, q);
    for (auto i = 0; i < q; i++) {
        cin >> oper;
        auto operator_val = OperatorType::query;
        if (oper == '?') {
            cin >> val_1;
            val_2 = 0;
        } 
        else {
            operator_val = OperatorType::increment;
            cin >> val_1 >> val_2;
        }
        testCase.operations[i] = operator_val;
        testCase.operand_1[i] = val_1;
        testCase.operand_2[i] = val_2;
    }
    return testCase;
}

/// @brief Calculate the prefix sum for a test problem.
/// @param n the number of elements in the array.
/// @param operations a list of operations to perform, updates or queries.
/// @param operand_1 list of first operands for the operations.
/// @param operand_2 list of second operands for the operations. Is expected
/// to be of the same length as operand_1. 
/// @param result a list of query results for the query operations.
void fenwick(int n,
            std::vector<OperatorType>& operations, 
             std::vector<int>& operand_1,
             std::vector<int>& operand_2,
             std::vector<long long>& result) 
{
    Fenwick<long long> ft(n);

    for (std::vector<int>::size_type i = 0; i < operations.size(); i++) {
        auto operation = operations[i];
        if (operation == OperatorType::increment) {
            auto index = operand_1[i];
            auto val = operand_2[i];
            ft.add(index, val);
        }
        else {
            auto index = operand_1[i];
            auto query_result = ft.sum(index);
            result.push_back(query_result);
        }
    }
}

/// @brief Helper function to print the results. 
/// @param results a list of query results, i.e. prefix sums
void print_result(const std::vector<long long>& result) 
{
    for (auto val: result) {
        std::cout << val << "\n";
    }
}

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    std::vector<long long> result;
    fenwick(testCase.N, testCase.operations, testCase.operand_1, testCase.operand_2, result);
    print_result(result);
    std::cout.flush();
    return 0;
}
