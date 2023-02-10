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
#include <set>
#include <map>

std::ifstream cin("in_chopwood.txt");
//auto& cin = std::cin;
using Vec = std::vector<int>;

struct TestCase {
    TestCase(int n) : n(n), v_vec(n) {
        
    }
    int n;

    Vec v_vec;   
};

TestCase process_input() 
{
    int n;
    int v;
    cin >> n;
    TestCase testCase(n);
    for (auto i = 0; i < n; i++) {
        cin >> v;
        testCase.v_vec[i] = v - 1;
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

void print_result(const Vec& result) 
{
    if (result.size() == 0) {
        std::cout << "Error\n";
    }
    for (auto nr : result) {
        std::cout << (nr + 1) << "\n";
    }
}

void chopwood(int n, const Vec& v_vec, Vec& result) {
    // base case:
    if (n == 1) {
        if (v_vec[0] != 1) {
            return;
        }
        result.push_back(0);
        return;
    }

    DisjointSet find_leaves_set(n + 1);

    auto reference_elem = v_vec[n - 1];
    for (int i = 0; i < n; i++) {
        find_leaves_set.union_sets(v_vec[i], reference_elem);
    }

    std::set<int> candidates;
    std::map<int, int> final_position;

    for (int i = 0; i < n + 1; i++) {
        auto ref_elem = find_leaves_set.find_set(i);
        if (find_leaves_set.get_size(ref_elem) == 1) {
            candidates.insert(i);
        }
    }   
    DisjointSet built_tree(n + 1);

    int location = 0;
    for (auto elem: v_vec) {
        final_position[elem] = location;
        location++;
    }

    location = 0;
    for (auto elem: v_vec) {
        if (candidates.size() == 0) {
            result.clear();
            return;
        }
        built_tree.union_sets(elem, *candidates.begin());
        result.push_back(*candidates.begin());
        candidates.erase(candidates.begin());
        if (final_position[elem] == location && location != n - 1) {
            candidates.insert(elem);
        }
        location++;
    }

    // do checks.
    if (candidates.size() != 0) {
        result.clear();
        return;
    }
    
    // Build a list of initial leafs.
    for (int i = 0; i < n + 1; i++) {
        if (built_tree.get_edge_count(i) == 1) {
            candidates.insert(i);
        }
    }
    std::vector<int> verification_vector(n);

    // Do verify the tree is 'proper'.
    for (int i = 0; i < n; i++) {
        if (candidates.size() == 0) {
            result.clear();
            return;
        }
        auto next_candidate = *candidates.begin();
        if (built_tree.get_edge_count(next_candidate) != 1) {
            result.clear();
            return;
        }

        auto parent = built_tree.get_real_parent(next_candidate);
        if (parent == next_candidate) {
            result.clear();
            return;
        }
        if (built_tree.get_edge_count(parent) <= 1 && i < n - 1) {
            result.clear();
            return;
        }
        if (result[i] != next_candidate) {
            result.clear();
            return;
        }

        verification_vector[i] = parent;
        built_tree.remove_edge(parent, next_candidate);
        candidates.erase(candidates.begin());
        if (built_tree.get_edge_count(parent) == 1) {
            candidates.insert(parent);
        }
    }
    for (int i = 0; i < verification_vector.size(); i++) {
        if (verification_vector[i] != v_vec[i]) {
            result.clear();
            return;
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    Vec result;
    chopwood(testCase.n, testCase.v_vec, result);
    print_result(result);
    
    std::cout.flush();
}