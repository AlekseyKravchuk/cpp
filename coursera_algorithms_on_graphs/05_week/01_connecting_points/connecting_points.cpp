#include <algorithm>
#include <cmath> // std::pow
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric> // std::iota
#include <tuple>
#include <vector>

using namespace std;

class disjoint_set {
  public:
    explicit disjoint_set(int64_t n);
    int64_t find(int64_t p);
    bool merge(int64_t x, int64_t y);

  private:
    vector<int64_t> _id;
    vector<int64_t> _rank;
    int64_t _size;
};

// ============================================

disjoint_set::disjoint_set(int64_t n) {
    _size = n;

    _id.resize(n);
    _rank.resize(n, 1);
    std::iota(_id.begin(), _id.end(), 0);
}

int64_t disjoint_set::find(int64_t p) {
    int64_t root = p;

    //find the root
    while (root != _id[root]) {
        root = _id[root];
    }

    //perform path compression
    while (p != root) {
        int64_t newp = _id[p];
        _id[p] = root;
        p = newp;
    }

    return root;
}

bool disjoint_set::merge(int64_t x, int64_t y) {
    x = find(x);
    y = find(y);

    if (x == y) {
        return false;
    }

    if (_rank[x] > _rank[y]) {
        _id[y] = x;
    } else {
        _id[x] = y;
    }

    if (_rank[x] == _rank[y]) {
        _rank[y] += 1;
    }

    return true;
}

// ============================================

double min_dist(int& n, vector<tuple<int, int, double>>& edges) {
    int u, v;
    double w, ans = 0.0;
    disjoint_set ds(n);

    std::sort(edges.begin(),
              edges.end(),
              [](const tuple<int, int, double>& lhs,
                 const tuple<int, int, double>& rhs) {
                  return get<2>(lhs) < get<2>(rhs);
              });

    for (const auto& e: edges) {
        tie(u, v, w) = e;

        if (ds.find(u) != ds.find(v)) {
            ds.merge(u, v);
            ans += w;
        }
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, x, y;
    cin >> n;

    vector<pair<int, int>> points;

    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        points.emplace_back(x, y);
    }

    vector<tuple<int, int, double>> edges;

    for (int i = 0; i < n; i++) {
        int x0 = points[i].first, y0 = points[i].second;

        for (int j = i + 1; j < n; j++) {
            int x = points[j].first;
            int y = points[j].second;

            double dist = sqrt(std::pow((double) (x - x0), 2) + std::pow((double) (y - y0), 2));

            edges.emplace_back(i, j, dist);
        }
    }

    cout << setprecision(10) << min_dist(n, edges);

    return 0;
}
