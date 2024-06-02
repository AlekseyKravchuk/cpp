#pragma once

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <memory>  // std::hash
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>  // std::move, std::pair
#include <vector>

struct Stop {
    std::string stop_name;      // название остановки
    double latitude_deg;        // широта в градусах
    double longitude_deg;       // долгота в градусах

    double latitude_rad;        // широта в радианах
    double longitude_rad;       // долгота в радианах

    Stop(std::string_view stop_name_, double latitude_deg_, double longitude_deg_)
            : stop_name(std::string(stop_name_)),
              latitude_deg(latitude_deg_),
              longitude_deg(longitude_deg_) {
        latitude_rad = latitude_deg * 3.1415926535 / 180;
        longitude_rad = longitude_deg * 3.1415926535 / 180;
    }
};

// statistics for a given bus route
struct BusRouteStats {
    struct BusRouteLengths {
        double geo_length{};
        unsigned long roads_length{};
    };

    size_t stops_count{0};
    size_t unique_stops_count{0};
    BusRouteLengths lengths{0.0, 0u};

    static constexpr double REASONABLE_ERROR = 0.0001;

    bool operator==(const BusRouteStats& rhs) const {
        return stops_count == rhs.stops_count &&
                unique_stops_count == rhs.unique_stops_count &&
                fabs(lengths.geo_length - rhs.lengths.geo_length) < REASONABLE_ERROR;
    }
};

class TransportGuide {
  public:
    using BusRoute = std::vector<Stop*>;
    using StopNameToStopPtr = std::unordered_map<std::string_view, Stop*>;
    using BusNameToBusRoute = std::unordered_map<std::string_view, BusRoute*>;
    using BusNameToBusRouteStats = std::unordered_map<std::string_view, BusRouteStats>;

    using StopNameFrom = std::string_view;
    using StopNameTo = std::string_view;
    using DistanceByRoad = size_t;

    struct PairOfStopNameViewHasher {
        size_t operator()(const std::pair<std::string_view, std::string_view>& p) const {
            auto seed = std::hash<std::string_view>{}(p.second);
            return std::hash<std::string_view>{}(p.first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
    };

    struct PairOfStopPointersHasher {
        size_t operator()(const std::pair<Stop*, Stop*>& p) const {
            auto seed = std::hash<Stop*>{}(p.second);
            return std::hash<Stop*>{}(p.first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
    };

    using EndStopsToDistances = std::unordered_map<StopNameTo, DistanceByRoad>;
    using DistancesTable = std::unordered_map<std::pair<Stop*, Stop*>,
            DistanceByRoad,
            PairOfStopPointersHasher>;

  public:
    TransportGuide() = default;
    void ProcessCreationQueries(std::istream& in);
    void ProcessRetrievalQueries(std::istream& in, std::ostream& out = std::cout);
    void PrintBusRouteInfo(std::string_view bus_route_name, std::ostream& out = std::cout) const;
    void PrintStopInfo(std::string_view stop_name, std::ostream& out = std::cout) const;

    const std::vector<std::string>& GetBuffer() const;
    const std::deque<Stop>& GetStops() const;
    const StopNameToStopPtr& GetStopNameToStopPtr() const;

    const std::unordered_map<std::pair<Stop*, Stop*>,
            DistanceByRoad,
            PairOfStopPointersHasher>& GetDistancesTable() const;
    size_t GetUniqueStopsCountForBusRoute(std::string_view bus_route_name) const;

    const std::deque<BusRoute>& GetBusRoutes() const;
    const BusNameToBusRoute& GetBusNameToBusRouteMapping() const;
    const BusRouteStats& GetStatsForBusRoute(std::string_view bus_route_name) const;

    std::pair<Stop, std::optional<EndStopsToDistances>> ParseAddStopQuery(std::string_view s);
    void CreateDistancesTable();

  private:
    // returns pair{bus_route_name, vector_of_stop_names}
    static std::pair<std::string_view, std::vector<std::string_view>> ParseBusRouteView(std::string_view bus_route_entry);

    void ProcessCreateQuery(std::string_view s);
    void AddStops(std::istream& in);
    void AddBusRoutes();

  private:
    enum class CreateCommand : char {
        Bus,
        Stop
    };

    std::unordered_map<std::string_view, CreateCommand> str_to_create_command = {
            {"Bus",  CreateCommand::Bus},
            {"Stop", CreateCommand::Stop}
    };
    // =============================================
    enum class RetrieveCommand : char {
        Bus,
        Stop
    };

    std::unordered_map<std::string_view, RetrieveCommand> str_to_retrieve_command = {
            {"Bus",  RetrieveCommand::Bus},
            {"Stop", RetrieveCommand::Stop}
    };
    // =============================================

    // Мы не можем одновременненно сохранять информацию об остановках и о маршрутах, состоящих из этих остановок.
    // Сначала нужно сформировать структуры данных из остановок, и только потом приступать к обработке маршрутов.
    std::vector<std::string> _buffer;              // buffer for storing whole records about bus routes from input

    std::deque<Stop>  _stops;                      // std::deque doesn't invalidate refs after insertion
    StopNameToStopPtr _stop_name_to_stop_ptr;      // hash table: stop name(string_view) => address of "Stop" instance

    std::deque<BusRoute> _bus_routes;              // each element of deque is vector<Stop*>
    BusNameToBusRoute _bus_name_to_bus_route;      // hash table: bus name(string_view) => address of "BusRoute" instance
    BusNameToBusRouteStats _bus_name_to_bus_route_stats;  // statistics on a given bus route

    // =========== Transport Guide, part B ===========
    std::unordered_map<std::string_view, std::set<std::string_view>> _stop_name_to_bus_routes;

    // =========== Transport Guide, part B ===========

    std::unordered_map<std::pair<Stop*, Stop*>, size_t, PairOfStopPointersHasher> _stops_pair_to_distance;
    std::unordered_set<std::string> _stop_names;

    // =========== Transport Guide, part C ===========
    std::unordered_map<std::pair<StopNameFrom, StopNameTo>,
                       DistanceByRoad,
                       PairOfStopNameViewHasher> _pair_stop_names_view_to_distance;

    DistancesTable _distances_table;

};

