#pragma once

#include <algorithm>
#include <deque>
#include <memory>  // std::shared_ptr
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern std::unordered_set<std::string> query_types;

extern const size_t MAX_CREATE_QUERIES;
extern const size_t MAX_COMMAND_LENGTH;

struct Stop {
    std::string stop_name;  // название остановки
    double latitude;        // широта
    double longitude;       // долгота
};

struct StopsInfo {
    std::deque<Stop> stops;
    std::unordered_map<std::string_view, Stop*> stop_name_to_stop_ptr;
};

using StopNameToStopPtr = std::unordered_map<std::string_view, Stop*>;

class TransportGuide {
  public:
    using BusRoute = std::vector<std::shared_ptr<Stop>>;
    using BusNameToBusPtr = std::unordered_map<std::string_view, std::shared_ptr<BusRoute>>;

    TransportGuide() = default;
    void SetStops(StopsInfo&& info);

  private:
    std::deque<Stop> _stops;                   // std::deque doesn't invalidate refs after insertion
    StopNameToStopPtr _stop_name_to_stop_ptr;  // hash table: stop name(string_view) => address of "Stop" instance

    std::deque<BusRoute> _bus_routes;          // each element of deque is vector<shared_ptr<Stop>>
    BusNameToBusPtr _bus_name_to_bus_ptr;      // hash table: bus name(string_view) => address of "BusRoute" instance
};

