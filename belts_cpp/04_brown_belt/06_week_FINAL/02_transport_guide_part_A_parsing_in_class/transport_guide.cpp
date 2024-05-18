#include "transport_guide.h"
#include "parse.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <numeric>  // std::accumulate
#include <cmath>    // std::acos, std::sin

using namespace std;

void TransportGuide::AddStops(std::istream& in) {
    size_t create_queries_count;
    in >> create_queries_count >> std::ws;
    _buffer.reserve(create_queries_count);

    // полностью обрабатываем ТОЛЬКО ту часть входного потока, которая касается остановок
    string line{};
    for (size_t i = 0; i < create_queries_count; ++i) {
        getline(in, line);
        ParseInputEntry(line);
    }
}

void TransportGuide::AddBusRoutes() {
    if (_buffer.empty()) { return; }

    for (string_view bus_route_entry : _buffer) {
        auto [bus_route_name, delimited_stops] = SplitIntoTwoParts(bus_route_entry, ':');
        delimited_stops = trim_view(delimited_stops);

        size_t first_delim_pos = delimited_stops.find_first_of(">-");
        if (first_delim_pos == string_view::npos) { continue; }

        const char separator = delimited_stops[first_delim_pos];
        vector<string_view> stops = SplitBy(delimited_stops, separator);

        // in case of circular bus route: A - B - C <=> A > B > |C| > B > A
        if (separator == '-') {
            stops.resize(stops.size() * 2 - 1);
            std::copy_backward(next(stops.rbegin()), stops.rend(),
                               stops.end());
        }

        _bus_routes.emplace_back(stops.size());
        BusRoute& current_bus_route = _bus_routes.back();

        unordered_set<string_view> unique_stops;
        for (size_t i = 0; i < stops.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stops[i]];
            unique_stops.insert(stops[i]);
        }

        _unique_stops_for_bus_routes.reserve(_buffer.size());
        double route_length = 0.0;

        // TODO: implement calculating length for each route
        accumulate(current_bus_route.begin(),
                   current_bus_route.end(),
                   route_length,
                   [](double acc, const Stop* rhs) {
                       return acos(sin(lhs->latitude) * sin(rhs->latitude) +
                                   cos(lhs->latitude) * cos(rhs->latitude) *
                                   cos(abs(lhs->longitude - rhs->longitude)));
                   });

        _bus_name_to_busroute_ptr[bus_route_name] = &current_bus_route;
        _unique_stops_for_bus_routes.push_back(unique_stops.size());
    }
}

const deque<Stop>& TransportGuide::GetStops() const {
    return _stops;
}

const vector<string>& TransportGuide::GetBuffer() const {
    return _buffer;
}

const TransportGuide::StopNameToStopPtr& TransportGuide::GetStopNameToStopPtr() const {
    return _stop_name_to_stop_ptr;
}

const deque<TransportGuide::BusRoute>& TransportGuide::GetBusRoutes() const {
    return _bus_routes;
}

const TransportGuide::BusNameToBusRoute& TransportGuide::GetBusNameToBusRoutePtr() const {
    return _bus_name_to_busroute_ptr;
}

const std::vector<size_t>& TransportGuide::GetUniqueStopsForBusRoutes() const {
    return _unique_stops_for_bus_routes;
}

void TransportGuide::ParseInputEntry(std::string_view s) {
    s = trim_view(s);
    size_t first_space_pos = s.find_first_of(' ');
    string_view command = s.substr(0, first_space_pos);
    s.remove_prefix(first_space_pos);
    s = trim_view(s);

    switch (str_to_command[command]) {
        case CreateDBCommand::Bus:
            // складываем во временный буфер предобработанные запросы на создание маршрута (без слова "Bus")
            _buffer.emplace_back(s);
            break;
        case CreateDBCommand::Stop:
            _stops.push_back(ParseStopView(s));
            _stop_name_to_stop_ptr.insert({_stops.back().stop_name, &(_stops.back())});
            break;
    }
}

void TransportGuide::CreateDB(std::istream& in) {
    AddStops(in);
    AddBusRoutes();
}
