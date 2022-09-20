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
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    map<string, QueryType> QueryToTypeMapping = {
        {"NEW_BUS"s, QueryType::NewBus},
        {"BUSES_FOR_STOP"s, QueryType::BusesForStop},
        {"STOPS_FOR_BUS"s, QueryType::StopsForBus},
        {"ALL_BUSES"s, QueryType::AllBuses}};

    string query_as_str;
    is >> query_as_str;
    q.type = QueryToTypeMapping[query_as_str];

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

struct BusesForStopResponse {
    string busStopName;
    vector<string> busRouteNamesThroughStop;

    bool isEmpty() {
        if (busStopName.empty()) {
            return true;
        } else {
            return false;
        }
    }
};

bool operator==(const BusesForStopResponse& lhs, const BusesForStopResponse& rhs) {
    if (lhs.busStopName == rhs.busStopName && lhs.busRouteNamesThroughStop == rhs.busRouteNamesThroughStop) {
        return true;
    } else {
        return false;
    }
}


// TO DO !!!!
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

struct StopsForBusResponse {
    string busRouteName;
    vector<string> stop_names;
};

bool operator==(const StopsForBusResponse& lhs, const StopsForBusResponse& rhs) {
    if (lhs.busRouteName == rhs.busRouteName && lhs.stop_names == rhs.stop_names) {
        return true;
    } else {
        return false;
    }
}

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    os << "Bus "s << r.busRouteName << ":";
    for (const auto& name : r.stop_names) {
        os << " " << name;
    }

    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> busRoutes;
};

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

        // cout << "Queries have been read." << endl;
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
            BusesForStopResponse tmp = {stopName, _stopsToBuses.at(stopName)};
            // return BusesForStopResponse({stopName, _stopsToBuses.at(stopName)});
            return tmp;
        } else {
            return BusesForStopResponse({});
        }
    }

    StopsForBusResponse GetStopsForBus(const string& busRouteName) const {
        if (_busesToStops.count(busRouteName)) {
            return StopsForBusResponse({busRouteName, _busesToStops.at(busRouteName)});
        } else {
            return StopsForBusResponse({});
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

void TestBusesForStopQuery() {
    BusManager bm;
    bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
    bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
    bm.AddBus("24"s, {"Vnukovo"s, "Solntsevo"s, "Dno"s});
    const string stopToCheck = "Vnukovo"s;

    BusesForStopResponse bfsr = bm.GetBusesForStop(stopToCheck);
    BusesForStopResponse bfsrSample = {stopToCheck, {"32"s, "32K"s, "24"s}};

    assert(bfsr == bfsrSample);

    cout << "Test \"TestBusesForStopQuery\" successfully passed." << endl;
}

void TestStopsForBusQuery() {
    BusManager bm;

    bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});

    StopsForBusResponse sfbr = bm.GetStopsForBus("32"s);
    StopsForBusResponse sfbrSample{"32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s}};

    assert(sfbr == sfbrSample);

    cout << "Test \"TestStopsForBusQuery\" successfully passed." << endl;
}

void testAllBusesQuery() {
    BusManager bm;
    bm.ReadQueries();
    bm.ProcessAllQueries();
}

void TestBusManager() {
    TestBusesForStopQuery();
    TestStopsForBusQuery();
}

int main() {
    // TestBusManager();
    testAllBusesQuery();

    return 0;
}
