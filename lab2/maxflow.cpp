#include "algo_lib.h"

#include <bits/stdc++.h>
using namespace algo;

std::ifstream cin("in_maxflow.txt");
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
    }
   return testCase;
}

void print_result(adjacency_graph<FlowPayload>& the_graph, int t, int flow) 
{
    // Determine max flow.
    int edges_used = 0;
    // Determine edges used.
    for (auto node: the_graph) {
        for (auto edge: node) {
            if (edge.payload.backward == false && edge.payload.flow > 0) {
                edges_used++;
            }
        }
    }

    std::cout << the_graph.size() << " " << flow << " " << edges_used << "\n";
    for (auto node: the_graph) {
        for (auto edge: node) {
            if (edge.payload.backward == false && edge.payload.flow > 0) {
                std::cout << edge.source << " " << edge.target << " " << edge.payload.flow << "\n";
            }
        }
    }
}

int max_flow(FlowAdjacencyGraph& g, int s, int t) 
{
    FlowMatrixGraph mg(g.size(), std::vector<std::reference_wrapper<FlowEdge>>(g.size(), default_flow_edge));
    for (auto& node : g) {
        for (auto& edge : node) {
            mg[edge.source][edge.target] = edge;
        }
    }
    int flow = 0;
    while (true) {
        auto result = bfs_flow(g, s, t);
        if (result.second > 0) {
            flow += result.second;
            int target = t;
            while (true) {
                int source = result.first[target];
                auto& edge = mg[source][target].get();
                edge.weight -= result.second;
                edge.payload.flow += result.second;
                auto& back_edge = mg[target][source].get();
                back_edge.weight += result.second;
                back_edge.payload.flow -= result.second;
                target = source;
                if (source == s) {
                    break;
                }
            }
        }
        else {
            break;
        }
    }
    return flow;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto testCase = process_input();
    int flow = max_flow(testCase.g, testCase.s, testCase.t);
    print_result(testCase.g, testCase.t, flow);
    std::cout.flush();
}