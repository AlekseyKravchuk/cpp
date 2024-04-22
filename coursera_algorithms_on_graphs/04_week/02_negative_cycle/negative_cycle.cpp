#include <cstddef>
#include <cstdint>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

int negative_cycle(int& n, vector<vector<int64_t>>& edges) {
    vector<int64_t> dist(static_cast<size_t>(n+1), 1001);
    deque<int64_t> negative_nodes;
    dist[1] = 0;

    for (int i = 0; i < n; i++) {
        for (const vector<int64_t>& e: edges) {
            int64_t u = e[0];
            int64_t v = e[1];
            int64_t wi = e[2];

            if (dist[v] > dist[u] + wi) {
                dist[v] = dist[u] + wi;

                if (i == (n-1)) {
                    negative_nodes.push_back(v);
                }
            }
        }
    }

    return negative_nodes.empty()
           ? 0
           : 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int vertices_count, edges_count, u, v, weight;
    cin >> vertices_count >> edges_count;

    vector<vector<int64_t>> edges;

    for (int i = 0; i < edges_count; i++) {
        cin >> u >> v >> weight;
        edges.push_back({u, v, weight});
    }

    cout << negative_cycle(vertices_count, edges) << endl;

    return 0;
}