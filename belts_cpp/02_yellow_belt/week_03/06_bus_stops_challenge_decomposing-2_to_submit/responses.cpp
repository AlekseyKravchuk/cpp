#include "responses.h"

std::ostream& operator<<(std::ostream& os, const BusesForStopResponse& r) {
    return (r.buses_for_stop.empty()) ? os << "No stop"s : os << Join(r.buses_for_stop);
}

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r) {
    if (r.stops_for_bus.empty()) {
        os << "No bus"s;
    } else {
        bool isFirst = true;
        for (const auto& [stop, buses_list] : r.stops_for_bus) {
            if (!isFirst) {
                os << std::endl;
            }
            isFirst = false;
            os << "Stop " << stop << ": "s << Join(buses_list);
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const AllBusesResponse& r) {
    if (r.response.empty()) {
        os << "No buses"s;
    } else {
        bool isFirst = true;
        for (const auto& [bus, stop_names] : r.response) {
            if (!isFirst) {
                os << std::endl;
            }
            isFirst = false;
            os << "Bus "s << bus << ": " << Join(stop_names);
        }
    }
    return os;
}