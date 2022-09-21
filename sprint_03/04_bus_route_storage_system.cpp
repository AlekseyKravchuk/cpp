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

    void Clear() {
        type = QueryType::AllBuses;
        bus = ""s;
        stop = ""s;
        stops.clear();
    }
};

struct BusesForStopResponse {
    string busStopName;
    vector<string> busesThroughStop;

    bool isEmpty() const {
        if (busStopName.empty()) {
            return true;
        } else {
            return false;
        }
    }
};

struct StopsForBusResponse {
    vector<pair<string, string>> stop2BusesThrough;

    bool isEmpty() const {
        return (stop2BusesThrough.empty()) ? true : false;
    }
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
        for (vector<string>::const_iterator it = r.busesThroughStop.begin(); it != r.busesThroughStop.end(); ++it) {
            if (next(it) != r.busesThroughStop.end()) {
                os << *it << " ";
            } else {
                os << *it;
            }
        }
    }

    return os;
}

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.isEmpty()) {
        os << "No bus";
    } else {
        for (vector<pair<string, string>>::const_iterator it = r.stop2BusesThrough.begin(); it != r.stop2BusesThrough.end(); ++it) {
            if (next(it) != r.stop2BusesThrough.end()) {
                os << "Stop "s << (*it).first << ":" << (*it).second << endl;
            } else {
                os << "Stop "s << (*it).first << ":" << (*it).second;
            }
        }
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
        _busToStops[bus] = stopNames;

        for (const auto& stopName : stopNames) {
            _stopToBuses[stopName].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stopName) const {
        if (_stopToBuses.count(stopName)) {
            return BusesForStopResponse({stopName, _stopToBuses.at(stopName)});
        } else {
            return BusesForStopResponse({});
        }
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        vector<pair<string, string>> interchangesPerStop;

        if (_busToStops.count(bus)) {
            for (const auto& stopName : _busToStops.at(bus)) {  // итерируемся по всем остановкам данного автобусного маршрута "bus"

                if (_stopToBuses.count(stopName)) {
                    string interchangesHolder = ""s;

                    for (const auto& busAtStop : _stopToBuses.at(stopName)) {  // итерируемся по всем автобусным маршрутам, проходящим ч/з данную остановку
                        if (bus != busAtStop) {
                            interchangesHolder += " " + busAtStop;
                        }
                    }
                    if (interchangesHolder.empty()) {
                        interchangesHolder = " no interchange";
                    }
                    interchangesPerStop.emplace_back(stopName, interchangesHolder);
                }
            }
            return StopsForBusResponse{interchangesPerStop};
        } else {
            return StopsForBusResponse{};
        }
    }

    AllBusesResponse GetAllBuses() const {
        return {_busToStops};
    }

   private:
    map<string, vector<string>> _busToStops;   // bus routes to stop names mapping: {busRouteName: [stopName1, ..., stopNameN]}
    map<string, vector<string>> _stopToBuses;  // stop names to bus routes mapping: {stopName1: [busRouteName3, busRouteName5, ... busRouteName_i]}
    vector<Query> _queries;                    // storage of queries to process

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

