
#include <bits/stdc++.h>
std::ifstream cin("in_turbo.txt");
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

enum class OperatorType {
    increment,
    query
};

struct TestCase {
    TestCase(int n) : n(n), elements(n) {
        
    }

    int n;
    std::vector<int> elements;
};

TestCase process_input() 
{
    int n;
    int elem;
    cin >> n;
    TestCase testCase(n);
    for (auto i = 0; i < n; i++) {
        cin >> elem;
        testCase.elements[i] = elem;
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

void turbo(int n, std::vector<int>& elements, std::vector<int>& results)
{
    Fenwick<int> fenwick(n);
    for (int i = 1; i <= n; i++) {
        fenwick.add(i, 1);
    }

    std::unordered_map<int, int> locations;
    for (int i = 0; i < elements.size(); i++) {
        locations[elements[i]] = i + 1; // plus one for one-indexed locations.
    }

    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            auto move_number = n - (i / 2 - 1);
            auto move_number_location = locations[move_number];
            auto fs1 = n + 1 - fenwick.sum(move_number_location);
            auto fs2 = n + 1 - move_number;
            auto swaps = fs1 - fs2;
            results.push_back(swaps);
            fenwick.add(move_number_location + 1, -1);
           // fenwick.add(move_number, 1);
        }
        else {
            auto move_number = i / 2 + 1; 
            auto move_number_location = locations[move_number];
            auto fs1 = fenwick.sum(move_number_location);
            auto fs2 = move_number; //fenwick.sum(move_number);
            auto swaps = fs1 - fs2;
            results.push_back(swaps);
            fenwick.add(move_number_location, -1);
            fenwick.add(1, 1);
        }
    }
}

void print_result(std::vector<int>& result) 
{
    for (auto elem: result) {
        std::cout << elem << "\n";
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    std::vector<int> results;
    turbo(testCase.n, testCase.elements, results);
    print_result(results);
    std::cout.flush();
}
