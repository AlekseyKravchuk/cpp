#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

using namespace std;

class RouteManager {
  public:
    void AddRoute(int start, int finish) {
        _reachable_lists[start].insert(finish);
        _reachable_lists[finish].insert(start);
    }

    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        if (_reachable_lists.count(start) < 1) {
            return result;
        }

        const set<int>& reachable_stations = _reachable_lists.at(start);
        const auto finish_pos = reachable_stations.lower_bound(finish);

        if (finish_pos != end(reachable_stations)) {
            result = min(result, abs(finish - *finish_pos));
        }

        if (finish_pos != begin(reachable_stations)) {
            result = min(result, abs(finish - *prev(finish_pos)));
        }

        return result;
    }

  private:
    map<int, set<int>> _reachable_lists;
};

int main() {
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }

    return 0;
}
