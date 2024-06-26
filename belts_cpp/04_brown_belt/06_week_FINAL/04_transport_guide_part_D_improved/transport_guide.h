#pragma once

#include "json.h"

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

using CurrentStopName = std::string_view;
using NextStopName = std::string_view;
using DistanceByRoad = size_t;
using NextStopsToDistancesMap = std::unordered_map<NextStopName, DistanceByRoad>;

using BusRouteName = std::string_view;
using StopNamesOnBusRoute = std::vector<std::string_view>;

struct Stop {
    constexpr static const double PI = 3.1415926535;

    std::string_view stop_name_view;  // название остановки
    double latitude_deg;              // широта в градусах
    double longitude_deg;             // долгота в градусах

    double latitude_rad;              // широта в радианах
    double longitude_rad;             // долгота в радианах

    Stop(std::string_view stop_name_as_view, double latitude_in_degrees, double longitude_in_degrees)
            : stop_name_view(stop_name_as_view),
              latitude_deg(latitude_in_degrees),
              longitude_deg(longitude_in_degrees) {
        latitude_rad = latitude_deg * PI / 180;
        longitude_rad = longitude_deg * PI / 180;
    }
};

std::pair<Stop, std::optional<NextStopsToDistancesMap>> RetrieveStopFromJsonMap(const std::map<std::string, Json::Node>& stop_info_map);

// statistics for a given bus route
struct BusRouteStats {
    static constexpr double REASONABLE_ERROR = 0.0001;

    size_t stops_count{0};
    size_t unique_stops_count{0};
    double length_by_geo_coordinates{};
    unsigned long length_by_roads{};
    double curvature{};

    bool operator==(const BusRouteStats& rhs) const {
        return stops_count == rhs.stops_count &&
               unique_stops_count == rhs.unique_stops_count &&
               fabs(length_by_geo_coordinates - rhs.length_by_geo_coordinates) < REASONABLE_ERROR &&
               length_by_roads == rhs.length_by_roads;
    }
};

class TransportGuide {
  public:
    using BusRoute = std::vector<Stop*>;
    using StopNameToStopPtr = std::unordered_map<std::string_view, Stop*>;
    using BusNameToBusRoute = std::unordered_map<std::string_view, BusRoute*>;
    using BusNameToBusRouteStats = std::unordered_map<std::string_view, BusRouteStats>;

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

    using DistancesTable = std::unordered_map<std::pair<Stop*, Stop*>,
                                              DistanceByRoad,
                                              PairOfStopPointersHasher>;

  public:
    TransportGuide() = default;

    void AddStopsAndPreprocessBusRoutes(const std::vector<Json::Node>& create_requests);
    void CreateDistancesTable();
    void FinallyProcessBusRoutes();
    void CalculateBusRouteStatistics(BusRouteName bus_name,
                                     BusRoute& bus_route,
                                     const StopNamesOnBusRoute& stop_names,
                                     const std::unordered_set<std::string_view>& unique_stops);

    void CreateDataBaseFromJSON(const std::vector<Json::Node>& base_requests);

    void PrintBusStatsIntoJSON(const std::map<std::string, Json::Node>& request_map,
                               std::ostream& out_stream) const;

    void PrintStopStatsIntoJSON(const std::map<std::string, Json::Node>& request_map,
                               std::ostream& out_stream) const;

    void ProcessRetrieveQueries(const std::vector<Json::Node>& retrieve_requests,
                                std::ostream& out_stream = std::cout) const;

    const std::deque<Stop>& GetStops() const;
    const StopNameToStopPtr& GetStopNameToStopPtr() const;

    const std::unordered_map<std::pair<Stop*, Stop*>,
            DistanceByRoad,
            PairOfStopPointersHasher>& GetDistancesTable() const;
    size_t GetUniqueStopsCountForBusRoute(std::string_view bus_route_name) const;

    const std::deque<BusRoute>& GetBusRoutes() const;
    const BusNameToBusRoute& GetBusNameToBusRouteMapping() const;
//    const BusRouteStats& GetStatsForBusRoute(std::string_view bus_route_name) const;
    std::optional<std::reference_wrapper<const BusRouteStats>> GetStatsForBusRoute(std::string_view bus_route_name) const;

  private:
    std::pair<Stop,
         std::optional<NextStopsToDistancesMap>> GetStopFromJsonMap(const std::map<std::string, Json::Node>& request_as_json_map);

    std::pair<std::string_view,
              std::vector<std::string_view>> GetBusRouteFromJsonMap(const std::map<std::string, Json::Node>& request_as_json_map);

    static double CalculateGeoDistanceBetweenTwoStops(Stop* stop_1, Stop* stop_2);

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
    std::unordered_map<BusRouteName, StopNamesOnBusRoute> _bus_name_to_stop_names;  // temporary storage
    std::unordered_set<std::string> _stop_names;
    std::unordered_set<std::string> _bus_names;
    std::deque<Stop>  _stops;                      // std::deque doesn't invalidate refs after insertion
    StopNameToStopPtr _stop_name_to_stop_ptr;      // hash table: stop name(string_view) => address of "Stop" instance


    std::deque<BusRoute> _bus_routes;              // each element of the deque is vector<Stop*>
    BusNameToBusRoute _bus_name_to_bus_route;      // hash table: bus name(string) => address of "BusRoute" instance
    BusNameToBusRouteStats _bus_name_to_bus_route_stats;  // statistics on a given bus route

    std::unordered_map<std::string_view, std::set<std::string_view>> _stop_name_to_bus_routes;

    std::unordered_map<std::pair<Stop*, Stop*>, size_t, PairOfStopPointersHasher> _stops_pair_to_distance;

    std::unordered_map<std::pair<CurrentStopName, NextStopName>,
                       DistanceByRoad,
                       PairOfStopNameViewHasher> _pair_stop_names_view_to_distance;

    DistancesTable _distances_table;
};

