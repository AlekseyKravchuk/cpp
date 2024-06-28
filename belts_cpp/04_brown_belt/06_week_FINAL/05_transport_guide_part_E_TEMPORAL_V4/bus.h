#pragma once

#include "stop_manager.h"

#include <cstddef>
#include <optional>
#include <vector>
#include <string>
#include <unordered_set>
#include <memory>
#include <utility>

class BusRoute {
  public:
    using BusRouteName = std::string;

    BusRoute(BusRouteName bus_no, std::vector<std::string> stop_names);
    BusRoute() = default;

    BusRouteName Number() const { return _bus_route_name; }

    const auto& Stops() const { return _stops; }

    size_t UniqueStopNumber() const { return _stop_names.size(); }

    std::optional<std::pair<double, double>> RouteLength() { return _route_length; }

    bool ContainsStop(const std::string& stop_name) const { return _stop_names.count(stop_name); }

    void SetRouteLength(size_t road_length, double direct_length) { _route_length = {road_length, direct_length}; }

    static BusRoute CreateRawBusRoute(BusRouteName bus_no, const std::vector<std::string>& stop_names);
    static BusRoute CreateCyclicBusRoute(BusRouteName bus_no, const std::vector<std::string>& stop_names);
  private:
    BusRouteName _bus_route_name;
    std::vector<std::string> _stops;
    std::unordered_set<std::string> _stop_names;
    std::optional<std::pair<size_t, double>> _route_length;
};
