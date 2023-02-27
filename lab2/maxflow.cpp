#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

std::ifstream cin("in_minspantree.txt");
//auto& cin = std::cin;
Edge<FlowPayload> dummy_edge;

struct TestCase {
    TestCase(int n, int m, int s, int t) : n(n), m(m), s(s), t(t), g(n) {
        
    }
    int n;
    int m;
    int s;
    int t;

    adjacency_graph<FlowPayload> g;
    matrix_ref_graph<FlowPayload> ref_g;
};

TestCase process_input() 
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    TestCase testCase(n, m, s, t);


    int u, v, c;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> c;
        Edge<FlowPayload> forward_edge(u, v, c, FlowPayload(c, 0, false));
        Edge<FlowPayload> backward_edge(v, u, 0, FlowPayload(0, 0, true));
        testCase.g[u].push_back(forward_edge);
        testCase.g[v].push_back(backward_edge);
        testCase.ref_g[u][v] = forward_edge;
        testCase.ref_g[v][u] = backward_edge;
    }
   return testCase;
}

void print_result(adjacency_graph<FlowPayload>& the_graph, std::vector<Edge<FlowPayload>> edges_used, int target_flow) 
{
    std::cout << the_graph.size() << " " << target_flow << " " << edges_used.size() << "\n";
    for (auto edge: edges_used) {
        std::cout << edge.source << " " << edge.target << " " << edge.payload.flow << "\n";
    }
}

void max_flow(matrix_graph<FlowPayload>& g, int s, int t) 
{

}

std::pair<std::vector<Edge<FlowPayload>>, int> extract_result(matrix_graph<FlowPayload>& g, int t) 
{
    std::vector<Edge<FlowPayload>> edges_used;
    for (auto node: g) {
        for (auto edge: node) {
            if (edge.payload.flow > 0 && edge.payload.backward == false) {
                edges_used.push_back(edge);
            }   
        }
    }
    int total_flow = 0;
    auto target_node_edges = g[t];
    for (auto edge: target_node_edges) {
        total_flow += edge.payload.flow;
    }
    return std::make_pair(edges_used, total_flow);
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    max_flow(testCase.g, testCase.s, testCase.t);
    auto result = extract_result(testCase.g, testCase.t);
    print_result(testCase.g, result.first, result.second);
    std::cout.flush();
}