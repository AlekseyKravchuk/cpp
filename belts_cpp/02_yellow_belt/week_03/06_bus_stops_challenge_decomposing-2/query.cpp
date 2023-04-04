#include "query.h"
#include "test_runner.h"

std::map<std::string, QueryType> str2qtype = {
    {"NEW_BUS"s, QueryType::NewBus},
    {"BUSES_FOR_STOP"s, QueryType::BusesForStop},
    {"STOPS_FOR_BUS"s, QueryType::StopsForBus},
    {"ALL_BUSES"s, QueryType::AllBuses},
};

std::map<QueryType, std::string> qtype2str = {
    {QueryType::NewBus, "NEW_BUS"s},
    {QueryType::BusesForStop, "BUSES_FOR_STOP"s},
    {QueryType::StopsForBus, "STOPS_FOR_BUS"s},
    {QueryType::AllBuses, "ALL_BUSES"s},
};

std::istream& operator>>(std::istream& is, QueryType& qtype) {
    std::string type_as_str;
    is >> type_as_str;
    qtype = str2qtype[type_as_str];

    return is;
}

std::istream& operator>>(std::istream& is, Query& q) {
    is >> std::ws;
    if (is.peek() == '\n') {
        is.ignore(1);
    }
    is >> q.type;
    switch (q.type) {
        case QueryType::NewBus: {
            int num_stops;
            is >> q.bus >> num_stops;
            q.stops.resize(num_stops);
            for (int i = 0; i < num_stops; ++i) {
                is >> q.stops[i];
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
            // code
            break;
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Query& q) {
    return os << "<QueryType: "s << qtype2str[q.type] << ", "s
              << "bus: "s << q.bus << ", "s
              << "stop: "s << q.stop << ", "s
              << "stops: "s << q.stops << ">"s;
}

