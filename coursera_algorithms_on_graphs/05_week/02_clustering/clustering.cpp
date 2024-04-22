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

    // find the root
    while (root != _id[root]) {
        root = _id[root];
    }

    // perform path compression
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

double minDist(const vector<tuple<int64_t, int64_t, double>>& edges,
               int64_t n,
               int64_t k) {
    int64_t u, v, nEdges = 0;
    double w;
    disjoint_set ds(n);

    for (const tuple<int64_t, int64_t, double>& edge: edges) {
        tie(u, v, w) = edge;

        if (ds.find(u) != ds.find(v)) {
            if (nEdges == n - k) {
                return w;
            } else {
                nEdges++;

                ds.merge(u, v);
            }
        }
    }

    return 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int64_t n, k, x, y;
    cin >> n;

    vector<pair<int64_t, int64_t>> points;

    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        points.emplace_back(x, y);
    }

    cin >> k;

    int64_t x0, y0, x1, y1;
    vector<tuple<int64_t, int64_t, double>> edges;

    for (int i = 0; i < n; i++) {
        tie(x0, y0) = points[i];

        for (int j = i + 1; j < n; j++) {
            tie(x1, y1) = points[j];
            double dist = sqrt(pow((double) (x1 - x0), 2) + pow((double) (y1 - y0), 2));

            edges.emplace_back(i, j, dist);
        }
    }

    std::sort(begin(edges),
              end(edges),
              [](const tuple<int64_t, int64_t, double>& lhs,
                 const tuple<int64_t, int64_t, double>& rhs) {
                  return get<2>(lhs) < get<2>(rhs);
              });

    cout << setprecision(10) << minDist(edges, n, k);

    return 0;
}