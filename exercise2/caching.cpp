
#include <bits/stdc++.h>
std::ifstream cin("in_caching.txt");
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
    TestCase(int c, int n, int a) : c(c), n(n), a(a), accesses(a) {
        
    }

    int c;
    int n;
    int a;
    std::vector<int> accesses;
};

TestCase process_input() 
{
    int c, n, a;
    int access;
    cin >> c >> n >> a;
    TestCase testCase(c, n, a);
    for (auto i = 0; i < a; i++) {
        cin >> access;
        testCase.accesses[i] = access;
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

int caching(int c, int n, int a, std::vector<int>& accesses) 
{
    DisjointSet object_access(a);
    std::map<int, int> next_index_access_map;
    
    std::map<int, int> next_index_map;
    for (int i = a - 1; i >= 0; i--) {
        auto access = accesses[i];
        auto next_index_iter = next_index_map.find(access);
        if (next_index_iter != next_index_map.end()) {
            object_access.union_sets(next_index_iter->second, i);
        }
        next_index_map[access] = i;
    }

    int no_more_access_index = a;
    int result = 0;
    std::unordered_set<int> cache;
    for (auto i = 0; i < a; i++) {
        auto access = accesses[i];
        if (cache.find(access) == cache.end()) {
            if (cache.size() < c) {
                result++;
                cache.insert(access);
            }
            else {
                // we need to evict.
                auto evict_candidate = next_index_access_map.rbegin();
                cache.erase(evict_candidate->second);
                next_index_access_map.erase(evict_candidate->first);
                result++;
                cache.insert(access);          
            }
        }
        // update next_index
        auto next_index = object_access.get_real_parent(i);
        next_index_access_map.erase(i);
        
        if (next_index != i) {
            next_index_access_map[next_index] = access;
        }
        else {
            // doesn't really matter what the number is
            next_index_access_map[no_more_access_index++] = access; 
        }
    }
    
    return result;
}

void print_result(int result) 
{
    std::cout << result << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    auto result = caching(testCase.c, testCase.n, testCase.a, testCase.accesses);
    print_result(result);
    std::cout.flush();
}
