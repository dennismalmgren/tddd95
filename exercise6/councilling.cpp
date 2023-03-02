#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

std::ifstream cin("in_councilling.txt");
//auto& cin = std::cin;

struct TestCase {
    TestCase(int n, int m, int s, int t) : n(n), m(m), s(s), t(t), g(n), 
                                            capacity(n, std::vector<int>(n, 0)), 
                                            flow(n, std::vector<int>(n, 0)) {
    }

    int n;
    int m;
    int s;
    int t;
    adjacency_graph g;
    std::vector<std::vector<int>> flow;
    std::vector<std::vector<int>> capacity;
};

int process_header() 
{
    int T;
    cin >> T;
    return T;
}

void process_testcase(TestCase& testCase) 
{
    int u, v, c;
    for (int i = 0; i < testCase.m; i++) {
        cin >> u >> v >> c;
        Edge forward_edge(u, v, c);
        Edge backward_edge(v, u, 0);
        testCase.g[u].push_back(forward_edge);
        testCase.g[v].push_back(backward_edge);
        testCase.capacity[u][v] = c;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_cases = process_header();

    for (int i = 0; i < test_cases; i++) {
        int n, club_count;
        std::string resident, club, party;
        cin >> n;

        std::unordered_map<std::string, int> party_name_map;
        std::unordered_map<std::string, int> club_name_map;
        std::vector<std::string> resident_names;
        std::vector<std::string> party_names;
        std::vector<std::string> club_names;
        std::vector<int> party_affiliation(n);
        std::vector<std::vector<int>> club_affiliation(n);

        int party_id = 0;
        int club_id = 0;
        for (int row = 0; row < n; row++) {
            cin >> resident >> party >> club_count;
            resident_names.push_back(resident);

            int found_party_id = 0;
            auto party_it = party_name_map.find(party);
            if (party_it == party_name_map.end()) {
                found_party_id = party_id;
                party_name_map[party] = found_party_id;
                party_names.push_back(party);
                party_id++;
            }
            else {
                found_party_id = party_it->second;
            }

            party_affiliation[row] = found_party_id;

            for (int club_index = 0; club_index < club_count; club_index++) {
                cin >> club;
                int found_club_id = 0;
                auto club_it = club_name_map.find(club);
                if (club_it == club_name_map.end()) {
                    found_club_id = club_id;
                    club_name_map[club] = club_id;
                    club_names.push_back(club);
                    club_id++;
                }
                else {
                    found_club_id = club_it->second;
                }

                club_affiliation[row].push_back(found_club_id);
            }
        }

        int node_count = n + 2 + party_id + club_id;
        int max_capacity = std::ceil(club_id / 2.0) - 1;
        if (max_capacity == 0) {
            std::cout << "Impossible." << "\n";
            if (i != test_cases - 1) {
                std::cout << "\n";
            }
            continue;
        }
        adjacency_graph g(node_count);
        std::vector<std::vector<int>> capacity(node_count, std::vector<int>(node_count, 0));
        std::vector<std::vector<int>> flow(node_count, std::vector<int>(node_count, 0));
        
        int SOURCE_NODE_ID = node_count - 2;
        int SINK_NODE_ID = node_count - 1;
        
        // Set up edges for source to party
        // with max capacity
        for (int i = 0; i < party_id; i++) {
            int party_node_id = n + i;
            g[SOURCE_NODE_ID].push_back(Edge(SOURCE_NODE_ID, party_node_id, 1));
            g[party_node_id].push_back(Edge(party_node_id, SOURCE_NODE_ID, 1));

            capacity[SOURCE_NODE_ID][party_node_id] = max_capacity;
        }

        // Set up edges for party to resident
        // with capacity 1
        for (int i = 0; i < n; i++) {
            int party_node_id = n + party_affiliation[i];
            g[i].push_back(Edge(i, party_node_id, 1));
            g[party_node_id].push_back(Edge(party_node_id, i, 1));
            capacity[party_node_id][i] = 1;
        }

        // Set up edges for resident to club
        // with capacity 1
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < club_affiliation[i].size(); j++) {
                int club_node_id = n + party_id + club_affiliation[i][j];
                g[i].push_back(Edge(i, club_node_id, 1));
                g[club_node_id].push_back(Edge(club_node_id, i, 1));
                capacity[i][club_node_id] = 1;
            }
        }

        // Set up edges for club to sink
        // with capacity 1
        for (int i = 0; i < club_id; i++) {
            int club_node_id = n + party_id + i;
            g[club_node_id].push_back(Edge(club_node_id, SINK_NODE_ID, 1));
            g[SINK_NODE_ID].push_back(Edge(SINK_NODE_ID, club_node_id, 1));
            capacity[club_node_id][SINK_NODE_ID] = 1;
        }
        
        int total_flow = max_flow(g, capacity, flow, SOURCE_NODE_ID, SINK_NODE_ID);
        if (total_flow != club_id) {
            std::cout << "Impossible." << "\n";
            if (i != test_cases - 1) {
                std::cout << "\n";
            }
            continue;
        }
        else {
            for (int i = 0; i < club_id; i++) {
                int club_node_id = n + party_id + i;
                for (int j = 0; j < n; j++) {
                    if (flow[j][club_node_id] > 0) {
                        std::cout << resident_names[j] << " " << club_names[i] << "\n";
                    }
                }
            }           
        }
        if (i != test_cases - 1) {
            std::cout << "\n";
        }
    }
    std::cout.flush();
}