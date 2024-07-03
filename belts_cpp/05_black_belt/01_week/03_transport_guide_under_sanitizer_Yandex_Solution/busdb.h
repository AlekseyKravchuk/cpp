 #pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <optional>
#include <string_view>

#include "geometry.h"
#include "router.h"

#define WEIGHTSPRECISION float


struct Stop {
  std::string name;
  Point location;
  std::unordered_map<std::string,uint> measuredDistances;
};


struct Route {
  std::string number;
  std::vector<std::string> stops;

  uint uniqueStops;

  std::optional<double> geographicalDistance;
  std::optional<uint> measuredDistance;

  uint stopsOnRoute() const {
    return stops.size();
  }
};

struct RoutingSettings {
  uint bus_wait_time;
  double bus_velocity;
};

class BusDB {


  struct StopInternal {
   Point location;
   std::set<std::string_view> linkedRoutes;
   std::unordered_map<uint,uint> measuredDistances;
  private:
      uint vertex_id_ = -1;
      friend BusDB;
  };
    uint getVertexId(std::pair<const std::string,StopInternal>* stopIt);
    uint getVertexId(const std::pair<const std::string,StopInternal>* stopIt) const;

  struct Distances {
      uint measured;
      double geographical;
  };

  struct RouteInternal {
      std::string number;
      std::vector<size_t> stops;
      uint uniqueStops;
      mutable Distances distances;
  };

  std::unordered_map<std::string,StopInternal> allStops;
  std::unordered_map<std::string,RouteInternal> routes;

  Distances computeDistancesOnRoute(const RouteInternal& route, size_t fromStopN, size_t toStopN) const; //, bool measuredOnly
//  void computeRouteLength(const Route& route) const;
  uint measureDistanceOnRoute(const RouteInternal& route, size_t fromStopN, size_t toStopN) const;

  RoutingSettings routing_settings_;

  std::optional<Graph::DirectedWeightedGraph<WEIGHTSPRECISION>> routesWeights;
  std::optional<Graph::Router<WEIGHTSPRECISION>> routesRouter;
  std::vector<std::pair<const std::string,StopInternal>*> stopsIds;
  struct EdgeInfo {
    RouteInternal* route;
    size_t from;
    size_t to;
  };
  std::vector<BusDB::EdgeInfo> EdgeIds;

public:

  std::optional<Stop> FindStop(std::string stopName) const;
  std::optional<std::set<std::string_view>> RoutesForStop(std::string stopName) const;
  std::optional<Route> FindRoute(std::string number) const;
  void PrepareRoutesRouter();
  struct PathPart {
    std::string_view routeNumber;
    std::string_view stopName;
    uint stopSpan;
    uint bus_wait_time;
    WEIGHTSPRECISION bus_ride_time;
  };
  PathPart toPathPart(Graph::EdgeId edge_id) const;
  std::optional<std::vector<PathPart>> ComputeFromTo(std::string fromStopName, std::string toStopName) const;

  // Input
  void AddStopInfo(Stop stop);
  void AddRoute(Route route);

  void SetRoutingSettings(RoutingSettings routing_settings);

  // enum class DataType {
  //   STOP,
  //   ROUTE,
  //   EMPTY
  // };

  // struct DataWrapper {
  //   DataWrapper() : type(DataType::EMPTY) {}
  //   DataWrapper(DataType type) : type_(type) {}
  //   const DataType type_;

  // };

  // struct StopWrapper : DataWrapper {
  //   StopWrapper() : DataWrapper(DataType::STOP);

  //   Stop data;
  // };

  // struct RouteWrapper : DataWrapper {
  //   RouteWrapper() : DataWrapper(DataType::ROUTE);

  //   Route data;
  // };

  // void AddGeneric(BusDB::DataWrapper data);


};
