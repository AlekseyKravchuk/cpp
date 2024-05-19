#pragma once

#include <algorithm>
#include <deque>
#include <string>
#include <sstream>  // std::istringstream
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>  // std::move
#include <cmath>

struct Stop {
    std::string stop_name;      // название остановки
    double latitude_deg;        // широта в градусах
    double longitude_deg;       // долгота в градусах

    double latitude_rad;        // широта в радианах
    double longitude_rad;       // долгота в радианах

    Stop(std::string stop_name_, double latitude_deg_, double longitude_deg_)
            : stop_name(std::move(stop_name_)),
              latitude_deg(latitude_deg_),
              longitude_deg(longitude_deg_) {
        latitude_rad = latitude_deg * 3.1415926535 / 180;
        longitude_rad = longitude_deg * 3.1415926535 / 180;
    }
};

struct BusRouteStats {
    size_t stops_count{0};
    size_t unique_stops_count{0};
    double bus_route_length{0.0};

    static constexpr double REASONABLE_ERROR = 0.0001;

    bool operator==(const BusRouteStats& rhs) const {
        return stops_count == rhs.stops_count &&
                unique_stops_count == rhs.unique_stops_count &&
                fabs(bus_route_length - rhs.bus_route_length) < REASONABLE_ERROR;
    }
};

class TransportGuide {
  public:
    using BusRoute = std::vector<Stop*>;
    using StopNameToStopPtr = std::unordered_map<std::string_view, Stop*>;
    using BusNameToBusRoute = std::unordered_map<std::string_view, BusRoute*>;
    using BusNameToBusRouteStats = std::unordered_map<std::string_view, BusRouteStats>;

  public:
    TransportGuide() = default;
    void CreateDB(std::istream& in);
    void ProcessRetrievalQueries(std::istream& in);

    const std::vector<std::string>& GetBuffer() const;

    const std::deque<Stop>& GetStops() const;
    const StopNameToStopPtr& GetStopNameToStopPtr() const;
    size_t GetUniqueStopsCountForBusRoute(std::string_view bus_route_name) const;

    const std::deque<BusRoute>& GetBusRoutes() const;
    const BusNameToBusRoute& GetBusNameToBusRouteMapping() const;
    const BusRouteStats& GetStatsForBusRoute(std::string_view bus_route_name) const;

  private:
    // returns pair{bus_route_name, vector_of_stop_names}
    static std::pair<std::string_view, std::vector<std::string_view>> ParseBusRouteView(std::string_view bus_route_entry);

    void ParseInputEntry(std::string_view);
    void AddStops(std::istream& in);
    void AddBusRoutes();

  private:
    enum class CreateDBCommand : char {
        Bus,
        Stop
    };

    std::unordered_map<std::string_view, CreateDBCommand> str_to_command = {
            {"Bus",  CreateDBCommand::Bus},
            {"Stop", CreateDBCommand::Stop}
    };

    // Мы не можем одновременненно сохранять информацию об остановках и о маршрутах, состоящих из этих остановок.
    // Сначала нужно сформировать структуры данных из остановок, и только потом приступать к обработке маршрутов.
    std::vector<std::string> _buffer;              // buffer for storing whole records about bus routes from input

    std::deque<Stop>  _stops;                      // std::deque doesn't invalidate refs after insertion
    StopNameToStopPtr _stop_name_to_stop_ptr;      // hash table: stop name(string_view) => address of "Stop" instance

    std::deque<BusRoute> _bus_routes;              // each element of deque is vector<Stop*>
    BusNameToBusRoute _bus_name_to_bus_route;      // hash table: bus name(string_view) => address of "BusRoute" instance
    BusNameToBusRouteStats _bus_name_to_bus_route_stats;
};

