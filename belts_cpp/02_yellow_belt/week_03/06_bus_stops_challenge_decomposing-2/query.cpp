#include "query.h"

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

std::map<std::string, QueryType> str2qtype = {
    {"NEW_BUS"s, QueryType::NewBus},
    {"BUSES_FOR_STOP"s, QueryType::BusesForStop},
    {"STOPS_FOR_BUS"s, QueryType::StopsForBus},
    {"ALL_BUSES"s, QueryType::AllBuses},
};
