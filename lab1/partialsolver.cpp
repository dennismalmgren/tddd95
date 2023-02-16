

// partialsolver.cpp
// Author: Dennis Malmgren
// Solves a linear equation system, or
// identifies whether it is inconsistent or has
// multiple solutions. When there are multiple solutions
// the variables that can be assigned are so.
#include <bits/stdc++.h>

std::ifstream cin("in_partialsolver.txt");
//auto& cin = std::cin;

const double PI = std::acos(-1);

/// @brief Potential results when solving
/// a linear system. Note that the solution 
/// can be partial.
enum class ResultType {
    solution,
    inconsistent
};

/// @brief Helper class to gather the result after trying to solve a system.
/// Uses variables to keep track of whether the solution was found
/// or if the system was inconsistent/had multiple solutions.
/// Free variables can be set to anything which has as a consequence
/// that there are multiple solutions.
/// Values in the vector result for indices in free_variables are undefined.
/// Other values in the result vector indicate the necessary value for those variables.
struct TestCaseResult {
    ResultType resultType;
    std::vector<double> result;
    std::set<int> free_variables;
};

/// @brief TestCase class
/// Gathers the necessary input to define a linear system problem
/// Has variables for the matrix A and matrix b in A x = b.
struct TestCase {
    TestCase(int n): A(n, std::vector<double>(n)), b(n) {
        
    }

    std::vector<std::vector<double>> A;
    std::vector<double> b;
};

/// @brief Input reading helper method
/// @return returns a complete test case by reading from stdin.
TestCase process_input() 
{
    int n;
    double a;
    cin >> n;
    TestCase testCase(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a;
            testCase.A[i][j] = a;
        }
    }
    for (int i = 0; i < n; i++) {
        cin >> a;
        testCase.b[i] = a;
    }
    return testCase;
}

/// @brief Helper function to print the results. Prints inconsistent
/// if the system is classified as such, otherwise the necessary 
/// value for each variable. For variables that can take on multiple
/// values, a '?' is written.
/// @param testCaseResult the results of solving a linear system.
void print_result(const TestCaseResult& testCaseResult) 
{ 
    switch (testCaseResult.resultType) {
        case ResultType::inconsistent:
            std::cout << "inconsistent\n";
            break;
        case ResultType::solution:
            for (int i = 0; i < testCaseResult.result.size(); i++) {
                if (testCaseResult.free_variables.find(i) != testCaseResult.free_variables.end()) {
                    std::cout << "? ";
                }
                else {
                    if (std::abs(testCaseResult.result[i]) < 1e-9) {
                        std::cout << 0 << " ";
                    }
                    else {
                        std::cout << testCaseResult.result[i] << " ";
                    }
                }
            }
            std::cout << "\n";
    }
}


/// @brief Solves the system Ax = b. A is assumed to be square.
/// Runs in O(n^3) time
/// @param A Matrix
/// @param b Answers
/// @return A test case result with variable values defined or the system
/// classified as inconsistent. The algorithm also identifies variables
/// that can take on multiple values.
TestCaseResult linear_solve(std::vector<std::vector<double>>& A, std::vector<double>& b) 
{
    int n = A.size();
    std::vector<int> where(n, -1);

    for (int row = 0, col = 0; row < n && col < n; col++) {
        auto pivot_row = row;
        for (int i = row + 1; i < n; i++) {
            if (std::abs(A[i][col]) > std::abs(A[pivot_row][col])) {
                pivot_row = i;
            }
        }
        if (std::abs(A[pivot_row][col]) < 1e-9) {
            continue;
        }
        for (int i = col; i < n; i++) {
            std::swap(A[pivot_row][i], A[row][i]);
        }
        std::swap(b[pivot_row], b[row]);
        where[col] = row;

        for (int i = 0; i < n; i++) {
            if (i != row) {
                double c = A[i][col] / A[row][col];
                for (int j = col; j < n; j++) {
                    A[i][j] -= A[row][j] * c;
                }
                b[i] -= b[row] * c;
            }
        }
        row++;
    }
        
    std::vector<double> value_result(n, 0);
    std::vector<int> free_indices;

    for (int i = 0; i < n; i++) {
        // normalize to one
        if (where[i] != -1) {
            value_result[i] = b[where[i]] / A[where[i]][i];
        }
    }
    
    TestCaseResult result;
    for (int i = 0; i < n; i++) {
        double the_sum = 0.0;
        // check that results are consistent.
        for (int j = 0; j < n; j++) {
            the_sum += value_result[j] * A[i][j];
        }
        if (std::abs(the_sum - b[i]) >= 1e-8) {
            result.resultType = ResultType::inconsistent;
            return result;
        }
    }    

    for (int i = 0; i < n; i++) {
        if (where[i] != -1) {
            for (int j = 0; j < n; j++) {
                if (j != i && std::abs(A[where[i]][j]) > 1e-8) {
                    where[i] = -1;
                    result.free_variables.insert(i);
                    break;
                }
            }
        }
        else {
            result.free_variables.insert(i);
        }
    }
    result.resultType = ResultType::solution;
    result.result = value_result;
    return result;
}

/// @brief Main method. Handles input reading, calling workhorse methods and 
/// result printing methods.
/// @return 0
int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    while (true) {
        auto testCase = process_input();
        if (testCase.A.size() == 0) {
            break;
        }
        auto result = linear_solve(testCase.A, testCase.b);
        print_result(result);
    }
  
    std::cout.flush();
    return 0;
}
