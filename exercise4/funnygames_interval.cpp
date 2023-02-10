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

std::vector<std::pair<double, double>> intersect_intervals(std::vector<std::pair<double, double>>& interval_candidates) {
    std::vector<std::pair<double, double>> result;
    if (interval_candidates.size() == 0) {
        return result;
    }
   
    auto next_candidate_start = interval_candidates[0].first;
    auto next_candidate_end = interval_candidates[0].second;

    for (int int_index = 1; int_index < interval_candidates.size(); int_index++) {
        auto& interval_candidate = interval_candidates[int_index];
        next_candidate_start = std::max(next_candidate_start, interval_candidate.first);
        next_candidate_end = std::min(next_candidate_end, interval_candidate.second);
    }
    if (next_candidate_start < next_candidate_end) {
        result.push_back(std::make_pair(next_candidate_start, next_candidate_end));
    }
    return result;
}

std::vector<std::pair<double, double>> union_intervals(std::vector<std::pair<double, double>>& interval_candidates) {
    std::vector<std::pair<double, double>> result;
    if (interval_candidates.size() == 0) {
        return result;
    }
    
    auto next_candidate_start = interval_candidates[0].first;
    auto next_candidate_end = interval_candidates[0].second;
    for (int int_index = 1; int_index < interval_candidates.size(); int_index++) {
        auto& interval_candidate = interval_candidates[int_index];
        if (interval_candidate.first <= next_candidate_end &&
            interval_candidate.second >= next_candidate_start) {
                next_candidate_start = std::min(next_candidate_start, interval_candidate.first);
                next_candidate_end = std::max(next_candidate_end, interval_candidate.second);
        }
        else {
            result.push_back(std::make_pair(next_candidate_start, next_candidate_end));
            next_candidate_start = interval_candidate.first;
            next_candidate_end = interval_candidate.second;
        }
    }
    result.push_back(std::make_pair(next_candidate_start, next_candidate_end));

    return result;
}

std::vector<std::vector<std::pair<double, double>>> combine(std::vector<std::vector<std::pair<double, double>>>& vecvec1, 
                                                    std::vector<std::pair<double, double>>& vec2)  {
    std::vector<std::vector<std::pair<double, double>>> result;
    for (auto vec1: vecvec1) {
        for (auto elem2: vec2) {
            result.push_back(std::vector<std::pair<double, double>>(vec1));
            result[result.size() - 1].push_back(elem2);
        }
    }
    return result;
}

std::vector<std::pair<double, double>> intersect_interval_lists(std::vector<std::vector<std::pair<double, double>>>& interval_lists) {
    std::vector<std::vector<std::pair<double, double>>> combinations;
    for (auto interv: interval_lists[0]) {
            combinations.push_back(std::vector<std::pair<double, double>>());
            combinations[combinations.size() - 1].push_back(interv);
    }

    for (int vec_index = 1; vec_index < interval_lists.size(); vec_index++) {
        combinations = combine(combinations, interval_lists[vec_index]);
    }
    std::vector<std::pair<double, double>> result;
    for (int vec_index = 0; vec_index < combinations.size(); vec_index++) {
        auto resulting_intervals = intersect_intervals(combinations[vec_index]);
        for (auto interval : resulting_intervals) {
            result.push_back(interval);
        }
    }
    return result;
}

std::vector<std::pair<double, double>> union_interval_lists(std::vector<std::vector<std::pair<double, double>>>& interval_lists) {
    std::vector<std::pair<double, double>> combinations;
    for (auto interval_list: interval_lists) {
        for (auto interv: interval_list) {
            combinations.push_back(interv);
        }
    }

    auto unioned = union_intervals(combinations);
    return unioned;
}

int funnygame(double x, int k, std::vector<double>& numbers)
{
    auto goal = std::log(x);
    std::sort(numbers.rbegin(), numbers.rend());
    std::transform(numbers.begin(), numbers.end(), numbers.begin(), [] (double x) {return -std::log(x);});
    auto win_interval = std::pair<double, double>(goal - numbers[numbers.size() - 1], goal);

    std::vector<std::pair<std::pair<double, double>, int>> intervals;
    intervals.push_back(std::make_pair(win_interval, 0));

    auto next_class = 1;
    while (intervals[0].first.first > numbers[0]) {
        auto first_interval = intervals[0].first;
        std::vector<std::vector<std::pair<double, double>>> interval_candidates;
        for (auto number : numbers) {
            std::vector<std::pair<double, double>> number_interval_candidates;
            for (int i = 0; i < intervals.size(); i++) {
                auto target_interval = intervals[i].first;
                auto target_interval_class = intervals[i].second;
                if (target_interval_class != 1 - next_class) {
                    continue;
                }
                auto interval_start_1 = target_interval.first - number;
                auto interval_start_2 = target_interval.second - number;
                interval_start_1 = std::min(interval_start_1, first_interval.first);
                interval_start_2 = std::min(interval_start_2, first_interval.first);
                if (interval_start_1 < interval_start_2 &&
                    interval_start_1 < first_interval.first) {
                        number_interval_candidates.push_back(std::pair<double, double>(interval_start_1, interval_start_2));
                    }
            }
            interval_candidates.push_back(number_interval_candidates);
        }
        if (next_class == 1) {
            auto final_intervals = intersect_interval_lists(interval_candidates);
            for (auto interval: final_intervals) {
                intervals.push_back(std::make_pair(interval, next_class));
            }
            std::sort(intervals.begin(), intervals.end(), [](auto elem1, auto elem2) { return elem1.first.first < elem2.first.first;});

        }
        else {
            auto final_intervals = union_interval_lists(interval_candidates);
            for (auto interval: final_intervals) {
                intervals.push_back(std::make_pair(interval, next_class));
            }
            std::sort(intervals.begin(), intervals.end(), [](auto elem1, auto elem2) { return elem1.first.first < elem2.first.first;});

        }
        auto intervals_start = intervals[0].first.first;
        double right_most_interval = intervals_start + numbers[numbers.size() - 1];
        auto it = std::find_if(intervals.begin(), intervals.end(), 
                    [&right_most_interval](const auto& elem) { return elem.first.first > right_most_interval;});
        if (it != intervals.end()) {
            intervals.erase(it, intervals.end());
        }

        next_class = 1 - next_class;
    }
    //first: can we find a winning move for Nils?
    for (auto number: numbers) {
        for (auto interval: intervals) {
            auto interval_begin = interval.first.first;
            auto interval_end = interval.first.second;
            auto interval_class = interval.second;
            if (number >= interval_begin &&
                number < interval_end &&
                interval_class == 1) {
                    return 0;
                }
        }
    }
    return 1;
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
