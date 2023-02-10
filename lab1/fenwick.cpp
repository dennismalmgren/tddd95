
#include <bits/stdc++.h>
std::ifstream cin("in_fenwick.txt");
//auto& cin = std::cin;

template<typename T>
inline T LSOne(T s) { 
    return s & (-s);
}

template <class T>
class Fenwick {
    public:
        using f_size_t = typename std::vector<T>::size_type;
        Fenwick(f_size_t n) : n (n), numbers(n + 1) 
        {
        }

        void add(f_size_t index, T number) 
        {
            index += 1;
            for (f_size_t ind = index; ind < numbers.size(); ind += LSOne((int)ind)) {
                numbers[ind] += number;
            }
        }

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

enum class OperatorType {
    increment,
    query
};

struct TestCase {
    TestCase(int n, int q) : N(n), Q(q), operations(q), operand_1(q), operand_2(q) {
        
    }

    int Q;
    int N;

    std::vector<OperatorType> operations;
    std::vector<int> operand_1;
    std::vector<int> operand_2;
};

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

void print_result(const std::vector<long long>& result) 
{
    for (auto val: result) {
        std::cout << val << "\n";
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    std::vector<long long> result;
    fenwick(testCase.N, testCase.operations, testCase.operand_1, testCase.operand_2, result);
    print_result(result);
    std::cout.flush();
}
