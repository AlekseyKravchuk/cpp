#include <iostream>
#include <utility>  // std::pair
#include <vector>
#include <queue>    // std::priority_queue
#include <limits>
#include <string>

using namespace std;

const uint64_t MAXVAL = std::numeric_limits<uint64_t>::max();

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    uint64_t vertices_count, edges_count, u, v, wi;
    cin >> vertices_count >> edges_count;

    vector<vector<pair<uint64_t, uint64_t>>> w(vertices_count + 1);

    for (uint64_t i = 0; i < edges_count; i++) {
        cin >> u >> v >> wi;
        w[u].emplace_back(v, wi);
    }

    cin >> u >> v;

    priority_queue<pair<uint64_t, uint64_t>,
            vector<pair<uint64_t, uint64_t>>,
            greater<>> q;
    vector<uint64_t> dist(vertices_count + 1, MAXVAL);

    dist[u] = 0;
    q.emplace(0, u);

    while (!q.empty()) {
        pair<uint64_t, uint64_t> node = q.top();
        q.pop();

        for (pair<uint64_t, uint64_t> next: w[node.second]) {
            uint64_t curDist = dist[node.second] + next.second;

            if (curDist < dist[next.first]) {
                dist[next.first] = curDist;
                q.emplace(dist[next.first], next.first);
            }
        }
    }

    string result = (dist[v] == MAXVAL)
                    ? "-1"
                    : to_string(dist[v]);

    cout << result << endl;

    return 0;
}