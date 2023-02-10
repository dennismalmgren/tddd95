#include <bits/stdc++.h>
std::ifstream cin("in_funnygames.txt");
//auto& cin = std::cin;

int max_cache_depth = 20;
struct TestCase {
    TestCase(double x, int k) : x(x), k(k), numbers(k) {
        
    }

    double x;
    int k;
    std::vector<double> numbers;
};

int process_header() 
{
    int n;
    cin >> n;
    return n;
}

TestCase process_input() 
{
    double x;
    int k;
    double number;

    cin >> x >> k;
    TestCase testCase(x, k);
    for (auto i = 0; i < k; i++) {
        cin >> number;
        testCase.numbers[i] = number;
    }
    return testCase;
}

int run_game(int turn, 
            int depth,
            int selected_number_index,
            long state,
            double goal, 
            std::vector<double>& numbers, 
            std::map<long long, bool>& winner) {    
    // return turn if current actor wins, using number as an action

    auto next_turn = 1 - turn; 
    goal -= numbers[selected_number_index];
    state = state + (1 << selected_number_index*6);
    if (goal <= 0) {
        return next_turn;
    }

    auto winner_iter = winner.find(state);
    if (winner_iter != winner.end()) {
        if (winner_iter->second == true) {
            return turn;
        } else {
            return next_turn;
        }
    }

    for (int selected_number = 0; selected_number < numbers.size(); selected_number++) {
        if (run_game(next_turn, depth + 1, selected_number, state, goal,  numbers, winner) == next_turn) {
            winner[state] = false;
            return next_turn;
        }
    }
    winner[state] = true;
    return turn;
}

int funnygame(double x, int k, std::vector<double>& numbers)
{
    double min_steps = 1/500.0;
    double max_steps = -std::log(500)/std::log(0.9);
    double max_children = std::pow(6.0, 58.0);
    long long unique_nodes = 61*31*13*11*9*8*5;

    auto goal = std::log(x);
    std::sort(numbers.begin(), numbers.end());
    std::transform(numbers.begin(), numbers.end(), numbers.begin(), [] (double x) {return -std::log(x);});
    goal -= numbers[0];
    auto turn = 0;
    std::map<long long, bool> winner; // we now track rounds instead.
    int state = 0;
    int depth = 0;

    for (int number_index = 0; number_index < numbers.size(); number_index++) {
        if (run_game(turn, depth, number_index, state, goal, numbers, winner) == turn) {
            return turn;
        }
    }
    return 1 - turn;
}

void print_result(int result) 
{
    if (result == 0) {
        std::cout << "Nils\n";
    }
    else {
        std::cout << "Mikael\n";
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCases = process_header();
    for (int i = 0; i < testCases; i++) {
        auto testCase = process_input();
        auto winner = funnygame(testCase.x, testCase.k, testCase.numbers);
        print_result(winner);
    }

    std::cout.flush();
}