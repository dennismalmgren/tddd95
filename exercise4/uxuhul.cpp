#include <bits/stdc++.h>
std::ifstream cin("in_uxuhul.txt");
//auto& cin = std::cin;

template<typename T>
std::vector<size_t> argsort(const std::array<T, 8> &array) {
    std::vector<size_t> indices(array.size());
    std::iota(indices.begin(), indices.end(), 1);
    std::sort(indices.begin(), indices.end(),
              [&array](int left, int right) -> bool {
                  // sort indices according to corresponding array element
                  return array[left - 1] < array[right - 1];
              });

    return indices;
}

struct TestCase {
    TestCase(int m) : m(m), voting_preferences(m) {
        
    }

    int m;
    std::vector<std::array<int, 8>> voting_preferences;
};

int process_header() 
{
    int n;
    cin >> n;
    return n;
}

TestCase process_input() 
{
    int m;
    int o;
    
    cin >> m;
    TestCase testCase(m);
    for (auto i = 0; i < m; i++) {
        for (auto j = 0; j < 8; j++) {
            cin >> o;
            testCase.voting_preferences[i][j] = o;
        }
    }
    return testCase;
}

std::string voting(int m, std::vector<std::array<int, 8>> preferences, std::vector<std::set<int>>& possible_transitions) {
    std::map<int, std::string> options;
    options[1] = "NNN";
    options[2] = "NNY";
    options[3] = "NYN";
    options[4] = "NYY";
    options[5] = "YNN";
    options[6] = "YNY";
    options[7] = "YYN";
    options[8] = "YYY";
   
    std::map<int, std::set<int>> round_transitions;
    for (int i = 1; i <= 8; i++) {
        round_transitions[i] = std::set({i});
    }
    for (int voting_round = m - 1; voting_round >= 0; voting_round--) {
        std::set<int> unused_transitions({1, 2, 3, 4, 5, 6, 7, 8});
        auto the_preferences = preferences[voting_round];
        auto round_preferences = argsort(the_preferences);
        std::map<int, std::set<int>> next_round_transitions;

        for (auto target: round_preferences) {
            if (round_transitions.find(target) != round_transitions.end()) {
                // it's reachable.
                for (auto outcome: round_transitions[target]) {
                    for (auto candidate_transition : possible_transitions[outcome]) {
                        if (unused_transitions.find(candidate_transition) != unused_transitions.end()) {
                            if (next_round_transitions.find(target) == next_round_transitions.end()) {
                                next_round_transitions[target] = std::set<int>();
                            }
                            next_round_transitions[target].insert(candidate_transition);
                            unused_transitions.erase(candidate_transition);
                        }
                    }
                }
            }
        }
        round_transitions = next_round_transitions;
    }
    for (auto outcome: round_transitions) {
        if (outcome.second.find(1) != outcome.second.end()) {
            std::string result = options[outcome.first];
            return result;
        }
    }
    return "";
}


void print_result(std::string result) 
{
    std::cout << result;
    std::cout << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    std::vector<std::set<int>> transitions;
    transitions.push_back(std::set<int>()); // dummy
    transitions.push_back(std::set<int>({2, 3, 5}));
    transitions.push_back(std::set<int>({1, 4, 6}));
    transitions.push_back(std::set<int>({1, 4, 7}));
    transitions.push_back(std::set<int>({2, 3, 8}));
    transitions.push_back(std::set<int>({1, 6, 7}));
    transitions.push_back(std::set<int>({2, 5, 8}));
    transitions.push_back(std::set<int>({3, 5, 8}));
    transitions.push_back(std::set<int>({4, 6, 7}));
    

    int testCases = process_header();
    for (int i = 0; i < testCases; i++) {
        auto testCase = process_input();
        auto result = voting(testCase.m, testCase.voting_preferences, transitions);
        print_result(result);
    }

    std::cout.flush();
}