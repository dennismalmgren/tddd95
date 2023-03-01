#include <bits/stdc++.h>

std::ifstream cin("in_maxflow.txt");
//auto& cin = std::cin;


const int INF = std::numeric_limits<int>::max();
std::vector<std::vector<int>> flow_edges;
std::vector<std::vector<int>> capacity;
std::vector<std::vector<int>> adj;

// check if there is a flow from s to t
int bfs(int s, int t, std::vector<int>& parent) {
	std::vector<bool> visited(capacity.size(), false);
	std::queue<int> q;
	q.push(s);
	parent[s] = -1; // none
	visited[s] = true;

	while (!q.empty()) {
		int cur = q.front();
		q.pop();

		for (int n : adj[cur]) {
			if (!visited[n] && capacity[cur][n] > 0) {
				parent[n] = cur;
				visited[n] = true;
				q.push(n);
				if (n == t) {
					return true;
				}
			}
		}
	}
	return false;
}

int maxflow(int s, int t, int n) {
	std::vector<int> parent(n);
	int maxflow = 0;
	while (bfs(s, t, parent)) {
		int flow = INF;

		for (int cur = t; cur != s; cur = parent[cur]) {
			int p = parent[cur];
			flow = std::min(flow, capacity[p][cur]);
		}

		for (int cur = t; cur != s; cur = parent[cur]) {
			int p = parent[cur];
			flow_edges[p][cur] += flow;
			flow_edges[cur][p] -= flow;
			capacity[p][cur] -= flow;
			capacity[cur][p] += flow;
		}

		maxflow += flow;
	}
	return maxflow;
}

int main() {
	int n, m, s, t, u, v, c;
	cin >> n >> m >> s >> t;
	adj.resize(n);
	capacity.resize(n, std::vector<int>(n, 0)); // matrix
	flow_edges.resize(n, std::vector<int>(n, 0)); // matrix
	for (int i = 0; i < m; i++) {
		cin >> u >> v >> c;
		adj[u].push_back(v);
		adj[v].push_back(u);
		capacity[u][v] = c;
	}

	int result = maxflow(s, t, n);
	int count_edges = 0;
	std::vector<std::pair<int, int>> ans;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (flow_edges[r][c] > 0) {
				count_edges++;
				ans.push_back(std::make_pair(r, c));
			}
		}
	}

	std::cout << n << ' ' << result << ' ' << count_edges << '\n';
	for (std::pair<int, int> e : ans) {
		int u = e.first;
		int v = e.second;
		std::cout << u << ' ' << v << ' ' << flow_edges[u][v] << '\n';
	}

	return 0;
}