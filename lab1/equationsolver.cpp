
// equationsolver.cpp
// Author: Dennis Malmgren
// Solves a linear equation system, or
// identifies whether it is inconsistent or has
// multiple solutions
#include <bits/stdc++.h>


// Helper variables to easily swap between test input (for debuggging) and Kattis
std::ifstream cin("in_equationsolver.txt");
//auto& cin = std::cin;

const double PI = std::acos(-1);

/// @brief Potential results when solving
/// a linear system.
enum class ResultType {
    solution,
    inconsistent,
    multiple
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


/// @brief Helper function to print the results. Prints inconsistent/multiple
/// if the system is classified as such, otherwise the necessary 
/// value for each variable.
/// @param testCaseResult the results of solving a linear system.
void print_result(const TestCaseResult& testCaseResult) 
{ 
    switch (testCaseResult.resultType) {
        case ResultType::inconsistent:
            std::cout << "inconsistent\n";
            break;
        case ResultType::multiple:
            std::cout << "multiple\n";
            break;
        case ResultType::solution:
            for (auto val: testCaseResult.result) {
                std::cout << val << " ";
            }
            std::cout << "\n";
    }
}

/// @brief Solves the system Ax = b. A is assumed to be square.
/// Runs in O(n^3) time
/// @param A Matrix
/// @param b Answers
/// @return A test case result with variable values defined or the system
/// classified as inconsistent or having multiple solutions.
TestCaseResult linear_solve(std::vector<std::vector<double>>& A, std::vector<double>& b) 
{
    int n = A.size();
    
    for (int i = 0; i < n; i++) {
        auto pivot_row = i;
        for (int j = i + 1; j < n; j++) {
            if (std::abs(A[j][i]) > std::abs(A[pivot_row][i])) {
                pivot_row = j;
            }
        }

        if (pivot_row != i) {
            std::swap_ranges(A[pivot_row].begin(), A[pivot_row].end(), A[i].begin());
            std::swap(b[pivot_row], b[i]);
        }

        // Check 1. If the highest value in this column (downwards) is a 0, 
        // we move to the next row.
        // this also means we can't clear out values above this row, in this column.
        if (std::abs(A[i][i]) < 1e-9) {
            continue;
        }

        // We have a non-zero pivot element.
        // normalize the row.
        auto normal_factor = 1.0 / A[i][i];
        for (int j = i; j < n; j++) {
            A[i][j] *= normal_factor;
        }
        b[i] *= normal_factor;

        // clear out the column for subsequent rows
        // putting the matrix in upper triangular form.
        for (int j = i + 1; j < n; j++) {
            double ratio = -A[j][i];
            for (int k = i; k < n; k++) {
                A[j][k] += ratio * A[i][k];
            }
            b[j] += ratio * b[i];
        }

        // clear out the column for previous rows
        // putting the matrix in reduced R-E form
        for (int j = i - 1; j >= 0; j--) {
            double ratio = -A[j][i];
            for (int k = i; k < n; k++) {
                A[j][k] += ratio * A[i][k];
            }
            b[j] += ratio * b[i];
        }

    }

    // check for inconsistencies
    TestCaseResult result;
    bool found_variables = false;
    for (int i = n - 1; i >= 0; i--) {
        found_variables = false;
        for (int j = 0; j < n; j++) {
            if (std::abs(A[i][j]) > 1e-9) {
                found_variables = true;
                break;
            }

        }
        if (!found_variables) {
            if (std::abs(b[i]) > 1e-9) {
                result.resultType = ResultType::inconsistent;
                return result;
            }
            else {
                // multiple. return result.
                result.free_variables.insert(i);
            }
        }
    }

    // variables -only- depending on free variables
    // are dependent variables.

    // now work backwards.
    result.result = std::vector<double>(n);
    result.resultType = ResultType::solution;
    std::set<int> dependent_variables;
    for (int v_index = n - 1; v_index >= 0; v_index--) {
        bool ismultiple = (result.free_variables.find(v_index) != result.free_variables.end());
        if (ismultiple) {
            continue;
        }
        for (auto free_variable: result.free_variables) {
            if (std::abs(A[v_index][free_variable]) > 1e-9) {
                dependent_variables.insert(v_index);
            }
        }
        ismultiple = (dependent_variables.find(v_index) != dependent_variables.end());
        if (ismultiple) {
            continue;
        }
        result.result[v_index] = b[v_index];
    }
    for (auto dep_var: dependent_variables) {
        result.free_variables.insert(dep_var);
    }
    if (result.free_variables.size() > 0) {
        result.resultType = ResultType::multiple;
    }
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
