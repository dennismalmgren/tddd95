#include <bits/stdc++.h>
std::ifstream cin("in_birds.txt");
//auto& cin = std::cin;

using ll = long long;

struct TestCase {
    TestCase(ll l, ll d, ll n) : l(l), d(d), n(n), bird_locations(n) {
        
    }

    ll l;
    ll d;
    ll n;
    std::vector<ll> bird_locations;
};


TestCase process_input() 
{
    ll l, d, n;

    ll loc;    
    cin >> l >> d >> n;

    TestCase testCase(l, d, n);
    for (ll i = 0; i < n; i++) {
        cin >> loc;
        testCase.bird_locations[i] = loc;
    }
    return testCase;
}

void print_result(int result) 
{
    std::cout << result << "\n";
}

int place_birds(ll l, ll d, ll n, std::vector<ll>& bird_locations) 
{
    std::sort(bird_locations.begin(), bird_locations.end());
    ll placed_birds = 0;
    // special case.
    if (bird_locations.size() == 0) {
        ll start_loc = 6; 
        ll end_loc = l - 6;
        placed_birds++;
        auto k = (end_loc - start_loc) / d;
        placed_birds += k;
       
    }
    else {
        // from start to first bird:
        ll start_loc = 6;
        ll end_loc = bird_locations[0] - d;
        if (start_loc <= end_loc) {
            placed_birds++;
            auto k = (end_loc - start_loc) / d;
            placed_birds += k;
        }

        // from bird to bird
        for (int i = 0; i < bird_locations.size() - 1; i++) {
            start_loc = bird_locations[i] + d;
            end_loc = bird_locations[i + 1] - d;
            if (start_loc <= end_loc) {
                placed_birds++;
                auto k = (end_loc - start_loc) / d;
                placed_birds += k;
            }
        }
        // from last bird to end:
        start_loc = bird_locations[bird_locations.size() - 1] + d;
        end_loc = l - 6;
        if (start_loc <= end_loc) {
            placed_birds++;
            auto k = (end_loc - start_loc) / d;
            placed_birds += k;
        }
    }
        // Do first check.
    return placed_birds;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    auto testCase = process_input();
    auto result = place_birds(testCase.l, testCase.d, testCase.n, testCase.bird_locations);
    print_result(result);

    std::cout.flush();
}