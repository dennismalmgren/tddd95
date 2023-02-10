#include <bits/stdc++.h>
std::ifstream cin("in_setstack.txt");
//auto& cin = std::cin;

template<typename T>
inline T LSOne(T s) { 
    return s & (-s);
}

/// @brief Handles numbers in ranges [1, n].
/// @tparam T 
template <class T>
class Fenwick {
    public:
        using f_size_t = typename std::vector<T>::size_type;
        Fenwick(f_size_t n) : n (n), numbers(n + 1) 
        {
        }

        /// @brief Uses 1-indexing
        /// @param index 
        /// @param number 
        void add(f_size_t index, T number) 
        {
            for (f_size_t ind = index; ind < numbers.size(); ind += LSOne((int)ind)) {
                numbers[ind] += number;
            }
        } 

        /// @brief Handles 1-indexed and sums [1, end] inclusive.
        /// @param end 
        /// @return 
        T sum(f_size_t end) 
        {
            T result = 0;
            for (f_size_t ind = end; ind > 0; ind -= LSOne(ind)) {
                result += numbers[ind];
            }
            return result;
        }
    private:
        int n;
        int offset;
        std::vector<T> numbers;
};

enum class OperationType {
    push,
    dup,
    set_union,
    intersect,
    add
};

struct TestCase {
    TestCase(int n) : n(n), operations(n) {
        
    }

    int n;
    std::vector<OperationType> operations;
};

int process_header() 
{
    int t;
    cin >> t;
    return t;
}

TestCase process_input() 
{
    int n;
    std::string oper;

    cin >> n;
    TestCase testCase(n);
    for (auto i = 0; i < n; i++) {
        cin >> oper;
        auto operationType = OperationType::push;
        if (oper == std::string("PUSH")) {
            operationType = OperationType::push;
        } else if (oper == std::string("DUP")) {
            operationType = OperationType::dup;
        }
        else if (oper == std::string("UNION")) {
            operationType = OperationType::set_union;
        }
        else if (oper == std::string("INTERSECT")) {
            operationType = OperationType::intersect;
        }
        else if (oper == std::string("ADD")) {
            operationType = OperationType::add;
        }
        else {
            throw std::exception();
        }
        testCase.operations[i] = operationType;
    }
    return testCase;
}

class DisjointSet {
    // Add path compression
    // we just
    public:
        DisjointSet(int n) : n(n), parent(n), rank(n), 
        sizes(n, 1), max_elems(n), edge_counts(n), real_parent(n) {
            std::iota(parent.begin(), parent.end(), 0);
            std::iota(real_parent.begin(), real_parent.end(), 0);
            std::iota(max_elems.begin(), max_elems.end(), 0);
        }
        
        int find_set(int elem) {
            if (elem == parent[elem]) {
                return elem;
            }
            return parent[elem] = find_set(parent[elem]);
        }

        // Let elem2 be a child to elem1.
        void union_sets(int elem1, int elem2) {
            auto a = find_set(elem1);
            auto b = find_set(elem2);
            if (a != b) {
                if (rank[a] < rank[b]) {
                    std::swap(a, b);
                }
                parent[b] = a;
                real_parent[elem2] = elem1;
                edge_counts[elem2]++;
                edge_counts[elem1]++;
                sizes[a] = sizes[b] + sizes[a];
                max_elems[a] = std::max(max_elems[a], max_elems[b]);
                if (rank[a] == rank[b]) {
                    rank[a]++;
                }
            }
        }
        
        int get_real_parent(int elem) {
            return real_parent[elem];
        }

        int get_edge_count(int elem) {
            return edge_counts[elem];
        }

        int get_size(int elem) {
            auto ref = find_set(elem);
            return sizes[ref];
        }

        int max_elem(int elem) {
            auto ref = find_set(elem);
            return max_elems[ref];
        }

        void remove_edge(int elem1, int elem2) {
            edge_counts[elem1]--;
            edge_counts[elem2]--;
        }

    private:
        int n;
        std::vector<int> parent;
        std::vector<int> real_parent;
        std::vector<int> rank;
        std::vector<int> sizes;
        std::vector<int> max_elems;
        std::vector<int> edge_counts;
};

void set_stack(int n, std::vector<OperationType>& operations, std::vector<int>& results)
{
    DisjointSet parents(n);
    std::map<std::set<int>, int> elementIndices;
    int max_used_index = 0;
    std::vector<std::set<int>> stack;

    for (auto operation: operations) {
        if (operation == OperationType::push) {
            auto next = std::set<int>();
            stack.push_back(next);
            results.push_back(next.size());
        }
        else if (operation == OperationType::dup) {
            auto latest = stack[stack.size() - 1]; 
            stack.push_back(latest);
            results.push_back(latest.size());
        }
        else if (operation == OperationType::set_union) {
            auto latest = stack[stack.size() - 1];
            stack.pop_back();
            auto second_latest = stack[stack.size() - 1];
            stack.pop_back();
            latest.insert(second_latest.begin(), second_latest.end());
            stack.push_back(latest);
            results.push_back(latest.size());
        }
        else if (operation == OperationType::intersect) {
            auto latest = stack[stack.size() - 1];
            stack.pop_back();
            auto second_latest = stack[stack.size() - 1];
            stack.pop_back();
            std::set<int> intersection;
            std::set_intersection(latest.begin(), latest.end(),
                                    second_latest.begin(), second_latest.end(),
                                    std::inserter(intersection, intersection.begin()));
            stack.push_back(intersection);
            results.push_back(intersection.size());
        }
        else if (operation == OperationType::add) {
            // The fun part.
            auto latest = stack[stack.size() - 1];
            stack.pop_back();
            auto second_latest = stack[stack.size() - 1];
            stack.pop_back();
            // 1. convert latest to a unique number
            if (latest.size() == 0) {
                second_latest.insert(0);
            }
            else if (latest.size() == 1) {
                auto number = *latest.begin();
                auto parent_id = parents.get_real_parent(number);
                if (parent_id == number) {
                    parent_id = ++max_used_index;
                    parents.union_sets(parent_id, number);
                }
                second_latest.insert(parent_id);

            }
            else {
                // size larger than one
                auto find_ind = elementIndices.find(latest);
                if (find_ind != elementIndices.end()) {
                    // // great, we have an assigned number. now lets do the parent thing in the union set.
                    // auto parent_id = parents.get_real_parent(find_ind->second);
                    // if (parent_id == find_ind->second) {
                    //     parent_id = ++max_used_index;
                    //     parents.union_sets(parent_id, find_ind->second);
                    // }
                    second_latest.insert(find_ind->second);
                }
                else {
                    // we don't, so lets create a proxy.
                    elementIndices[latest] = ++max_used_index;
                    second_latest.insert(max_used_index);
                }
            }
            // 2. add it to second latest.
            stack.push_back(second_latest);
            results.push_back(second_latest.size());
        }
    } 
}

void print_result(std::vector<int>& result) 
{
    for (auto elem: result) {
        std::cout << elem << "\n";
    }
    std::cout << "***" << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCases = process_header();

    for (int i = 0; i < testCases; i++) {
        auto testCase = process_input();
        std::vector<int> results;
        set_stack(testCase.n, testCase.operations, results);
        print_result(results);
    }

    std::cout.flush();
}