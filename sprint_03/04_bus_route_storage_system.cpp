#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>  // for std::inserter
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus ;
    string stop;
    vector<string> stops;

    void Clear() {
        type = QueryType::AllBuses;
        bus = ""s;
        stop = ""s;
        stops.clear();
    }
};

struct BusesForStopResponse {
    string busStopName;
    vector<string> busRouteNamesThroughStop;

    bool isEmpty() const {
        if (busStopName.empty()) {
            return true;
        } else {
            return false;
        }
    }
};

struct StopsForBusResponse {
    map<string, string> stop2BusesThrough;
};

struct AllBusesResponse {
    map<string, vector<string>> busRoutes;
};

istream& operator>>(istream& is, Query& q) {
    q.Clear();
    map<string, QueryType> queryToTypeMapping = {
        {"NEW_BUS"s, QueryType::NewBus},
        {"BUSES_FOR_STOP"s, QueryType::BusesForStop},
        {"STOPS_FOR_BUS"s, QueryType::StopsForBus},
        {"ALL_BUSES"s, QueryType::AllBuses}};

    string query_as_str;
    is >> query_as_str;
    q.type = queryToTypeMapping[query_as_str];

    switch (q.type) {
        case QueryType::NewBus: {
            is >> q.bus;   // название маршрута
            is >> q.stop;  // количество остановок на маршруте

            for (int i = 0; i < stoi(q.stop); ++i) {
                string stop_name;
                is >> stop_name;
                q.stops.push_back(stop_name);  // q.stops: вектор с названиями остановок на маршруте "q.bus"
            }
            break;
        }
        case QueryType::BusesForStop:
            is >> q.stop;
            break;
        case QueryType::StopsForBus:
            is >> q.bus;
            break;
        case QueryType::AllBuses:
            break;
    }

    return is;
}

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.isEmpty()) {
        os << "No stop";
    } else {
        os << "Stop "s << r.busStopName << ":";
        for (const auto& stopName : r.busRouteNamesThroughStop) {
            os << " " << stopName;
        }
    }

    return os;
}

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    for (const auto& [stopName, busesThroughStop] : r.stop2BusesThrough) {
        os << "Stop "s << stopName << ":" << busesThroughStop << endl;
    }

    return os;
}

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.busRoutes.empty()) {
        os << "No buses";
    } else {
        for (const auto& [busRouteName, stopNames] : r.busRoutes) {
            os << "Bus "s << busRouteName << ":";

            for (const auto& name : stopNames) {
                os << " " << name;
            }

            os << endl;
        }
    }

    return os;
}

class BusManager {
   public:
    void ReadQueries() {
        int num_queries;
        cin >> num_queries;
        _queries.resize(num_queries);

        for (int i = 0; i < num_queries; ++i) {
            cin >> _queries[i];
        }
    }

    void ProcessAllQueries() {
        for (const auto& query : _queries) {
            ProcessQuery(query);
        }
    }

    void AddBus(const string& bus, const vector<string>& stopNames) {
        _busesToStops[bus] = stopNames;

        for (const auto& stopName : stopNames) {
            _stopsToBuses[stopName].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stopName) const {
        if (_stopsToBuses.count(stopName)) {
            return BusesForStopResponse({stopName, _stopsToBuses.at(stopName)});
        } else {
            return BusesForStopResponse({});
        }
    }

    // TO DO !!!!!!!!!!!!!
    // Переделать логику обработки с map <string, string> на vector<pair<string, string>>,
    // т.к. нужно сохранить первоначальный порядок следования
    StopsForBusResponse GetStopsForBus(const string& bus) const {
        map <string, string> dict;
        vector<pair<string, string>> interchangesPerStop;
        string s;

        if (_busesToStops.count(bus)) {
            for (const auto& stopName: _busesToStops.at(bus)) {
                if (_stopsToBuses.count(stopName)) {

                    for(const auto& busAtStop : _stopsToBuses.at(stopName)) {
                        if (bus != busAtStop) {
                            dict[stopName] += " " + busAtStop;
                            interchangesPerStop.emplace_back(stopName, );
                        }
                    }

                    if (dict[stopName].empty()) {
                        dict[stopName] = " no interchange";
                    }
                }
            }  
            return StopsForBusResponse{dict};
        } else {
            return StopsForBusResponse{};
        }
    }

    AllBusesResponse GetAllBuses() const {
        return {_busesToStops};
    }

   private:
    map<string, vector<string>> _busesToStops;  // bus routes to stop names mapping: {busRouteName: [stopName1, ..., stopNameN]}
    map<string, vector<string>> _stopsToBuses;  // stop names to bus routes mapping: {stopName1: [busRouteName3, busRouteName5, ... busRouteName_i]}
    vector<Query> _queries;                     // storage of queries to process

    void ProcessQuery(const Query& q) {
        switch (q.type) {
            case QueryType::NewBus:
                AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop: {
                cout << GetBusesForStop(q.stop) << endl;
                break;
            }
            case QueryType::StopsForBus:
                cout << GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << GetAllBuses() << endl;
                break;
        }
    }
};

void testAllBusesQuery() {
    BusManager bm;
    bm.ReadQueries();
    bm.ProcessAllQueries();
}

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;

        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}

// int main() {
//     testAllBusesQuery();

//     return 0;
// }
