#include <bits/stdc++.h>
std::ifstream cin("in_funnygames.txt");
//auto& cin = std::cin;

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

int union_intervals(std::vector<std::pair<double, double>>& interval_candidates, int start_index, int end_index) {
    if (end_index <= start_index) {
        return start_index;
    }

    auto insert_index = start_index;
    auto next_candidate_start = interval_candidates[start_index].first;
    auto next_candidate_end = interval_candidates[start_index].second;

    for (int int_index = start_index + 1; int_index < end_index; int_index++) {
        auto& interval_candidate = interval_candidates[int_index];
        if (interval_candidate.first <= next_candidate_end &&
            interval_candidate.second >= next_candidate_start) {
                //next_candidate_start = std::min(next_candidate_start, interval_candidate.first);
                next_candidate_end = std::max(next_candidate_end, interval_candidate.second);
        }
        else {
            interval_candidates[insert_index++] = std::make_pair(next_candidate_start, next_candidate_end);
            next_candidate_start = interval_candidate.first;
            next_candidate_end = interval_candidate.second;
        }
    }
    interval_candidates[insert_index++] = std::make_pair(next_candidate_start, next_candidate_end);
    return insert_index;
}

int expand_interval(std::vector<std::pair<double, double>>& interval_candidates, 
                    const std::pair<double, double>& win_interval,
                    int intervals_end_index, 
                    const std::vector<double>& numbers) {

    for (auto number: numbers) {
        if (intervals_end_index < interval_candidates.size()) {
            interval_candidates[intervals_end_index++] = std::make_pair(win_interval.first * number, win_interval.second * number);
        }
        else {
            interval_candidates.push_back(std::make_pair(win_interval.first * number, win_interval.second * number));
            intervals_end_index++;
        }
    }
    return intervals_end_index;
}

int funnygame(double x, int k, std::vector<double>& numbers)
{
    std::vector<int> vec;
    std::cout << "Starting" << std::endl;
    for (long long i = 0; i < 97348680; i++) {
        vec.push_back(0);
    }
    std::cout << "Done" << std::endl;
    auto goal = x;
    std::sort(numbers.rbegin(), numbers.rend());
    std::transform(numbers.begin(), numbers.end(), numbers.begin(), [] (double x) {return 1 / x;});

    auto win_interval = std::pair<double, double>(1, 1); // dummy init.

    std::vector<std::pair<double, double>> intervals;
    intervals.reserve(1000*numbers.size());
    auto intervals_end_index = 0;
    for (int i = 0; i < numbers.size(); i++) {
        std::pair<double, double> interval(1, numbers[i]);
        intervals.push_back(interval);
        intervals_end_index++;
    }

    auto expand_interval_index = 0;

    while (goal > win_interval.second) {
        auto interval_end_iterator = intervals.begin();
        std::advance(interval_end_iterator, intervals_end_index);

        std::sort(intervals.begin(), interval_end_iterator);
        intervals_end_index = union_intervals(intervals, expand_interval_index, intervals_end_index);
        auto furthest_loss_candidate = intervals[expand_interval_index].second * numbers[0];
        if (expand_interval_index < intervals_end_index - 1) {
            furthest_loss_candidate = std::min(furthest_loss_candidate, intervals[expand_interval_index + 1].first);
        }
        win_interval = std::make_pair(intervals[expand_interval_index++].second,
                                            furthest_loss_candidate);
        intervals_end_index = expand_interval(intervals, win_interval, intervals_end_index, numbers);
    }
    if (win_interval.first < goal)  {
        return 1;
    }
    else {
        return 0;
    }
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