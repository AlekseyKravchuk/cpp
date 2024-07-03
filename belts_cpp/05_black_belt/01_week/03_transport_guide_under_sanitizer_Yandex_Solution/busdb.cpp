#include "busdb.h"

#include <sstream>
#include <utility>
#include <numeric>

using namespace std;

uint BusDB::getVertexId(std::pair<const std::string,StopInternal>* stopIt) {
    if (stopIt->second.vertex_id_ == -1) {
        stopIt->second.vertex_id_ = stopsIds.size();
        stopsIds.push_back(stopIt);
    }
    return stopIt->second.vertex_id_;
}

uint BusDB::getVertexId(const std::pair<const std::string,StopInternal>* stopIt) const {
    if (stopIt->second.vertex_id_ == -1)
        throw logic_error("Wrong StopId for stop:" + stopIt->first);
    return stopIt->second.vertex_id_;
}

BusDB::Distances BusDB::computeDistancesOnRoute(const RouteInternal& route,
                                                size_t fromStopN, size_t toStopN) const { //, bool measuredOnly = false
    bool fullRoute = (fromStopN == 0 && toStopN+1 == route.stops.size());
    Distances result;
    result.geographical = 0;
    result.measured = 0;
    const vector<size_t>& stopsOnRoute = route.stops;
    auto from = stopsIds.at(stopsOnRoute.at(fromStopN));
    for (size_t i = fromStopN+1; i <= toStopN; i++) {
        auto to = stopsIds.at(stopsOnRoute.at(i));
        result.measured += from->second.measuredDistances.at(getVertexId(to));
        if (fullRoute) result.geographical += computeDistance(from->second.location,
                                               to->second.location) * 1000.0; // to meters;
        from = to;
    }
    if (fullRoute) {
        route.distances = result;
    }
    return result;
}

//void BusDB::computeRouteLength(const Route& route) const {
//  BusDB::Distances result = computeDistancesOnRoute(route,0,route.stopsOnRoute()-1);
//  route.geographicalDistance = result.geographical;
//  route.measuredDistances = result.measured;
//}

uint BusDB::measureDistanceOnRoute(const RouteInternal& route, size_t fromStopN, size_t toStopN) const {
//    if (route.measuredDistances && fromStopN == 0 && toStopN+1 == route.stopsOnRoute()) return route.measuredDistances.value();
    return computeDistancesOnRoute(route,fromStopN,toStopN).measured;
}

void BusDB::AddStopInfo(Stop stop) {
    auto [stopIt, isInserted] = allStops.try_emplace(stop.name);
    uint stopVertexId = getVertexId(&*stopIt);
  for (const auto& [destinationStop, distance] : stop.measuredDistances) {
      auto destinationStopIt = allStops.try_emplace(destinationStop).first;
    stopIt->second.measuredDistances[getVertexId(&*destinationStopIt)] = distance;
      destinationStopIt->second.measuredDistances.emplace(stopVertexId, distance);
  }
    stopIt->second.location =  stop.location;
}

void BusDB::AddRoute(Route route) {
  auto pos = routes.emplace(std::move(route.number),
                            RouteInternal{.number = route.number, .uniqueStops = route.uniqueStops,
                                         .distances ={.measured = route.measuredDistance.value_or(0),
                                                      .geographical = route.geographicalDistance.value_or(0)}}
                            ).first;
  pos->second.stops.reserve(route.stops.size());
  for (string& stop : route.stops) {
      auto destinationStopIt = allStops.try_emplace(std::move(stop)).first;
      pos->second.stops.emplace_back(getVertexId(&*destinationStopIt));
      destinationStopIt->second.linkedRoutes.emplace(pos->first);
  }
}


void BusDB::SetRoutingSettings(RoutingSettings routing_settings) {
  routing_settings_ = routing_settings;
}

// void BusDB::AddRoute(Route route) {
//   Route& route_ = routes[route.number] = move(route);
//   for (const string& stop : route_.stops) {
//     StopAndLinks& stopEntry = allStops.at(stop);
//     stopEntry.linkedRoutes.push_front(&route_);
//     if (next(stopEntry.linkedRoutes.begin()) == stopEntry.linkedRoutes.end())
//       route_.uniqueStops++;
//     else if (next(stopEntry.linkedRoutes.begin(),2) == stopEntry.linkedRoutes.end())
//       (*next(stopEntry.linkedRoutes.begin()))->uniqueStops--;
//   }
//}

optional<Stop> BusDB::FindStop(string stopName) const {
  optional<Stop> result;
  auto pos = allStops.find(stopName);
  if (pos != allStops.end()) {
      unordered_map<string, uint> measuerdDistances;
      for (auto [StopId, distance]: pos->second.measuredDistances)
          measuerdDistances[stopsIds.at(StopId)->first] = distance;
      result = {.name = std::move(stopName),
              .location = pos->second.location,
              .measuredDistances = std::move(measuerdDistances)};
  }
  return result;
}

optional<set<string_view>> BusDB::RoutesForStop(string stopName) const {
     optional<set<string_view>> result;
     auto pos = allStops.find(stopName);
     if (pos != allStops.end())
       result = pos->second.linkedRoutes;
     return result;
}

optional<Route> BusDB::FindRoute(string number) const {
    optional<Route> result;
  auto pos = routes.find(number);
  if (pos != routes.end()) {
//    if (!pos->second.measuredDistances) {
//        computeRouteLength(pos->second);
//    }
    vector<string> stops;
    stops.reserve(pos->second.stops.size());
    for (uint stopId : pos->second.stops) {
        stops.push_back(stopsIds.at(stopId)->first);
    }
    result = {.number = std::move(number),
              .stops = std::move(stops),
              .uniqueStops = pos->second.uniqueStops,
              .geographicalDistance = pos->second.distances.geographical,
              .measuredDistance = pos->second.distances.measured};
  }
  return result;
}

void BusDB::PrepareRoutesRouter() {
    if (routesRouter) return;

    routesWeights = Graph::DirectedWeightedGraph<WEIGHTSPRECISION>(allStops.size());
//    stopsIds.reserve(allStops.size());
    EdgeIds.reserve(max(routes.size(),allStops.size()/2));

//    uint id = 0;
//    for (auto &stopIterator: allStops) {
//        stopIterator.second.VertexId = id++;
//        stopsIds.push_back(&stopIterator);
//    }
    for (auto &routeIterator: routes) {
        for (size_t i = 0; i + 1 < routeIterator.second.stops.size(); i++) {
            Graph::Edge<WEIGHTSPRECISION> routePart;
            routePart.from = routeIterator.second.stops[i];
            for (size_t j = i + 1; j < routeIterator.second.stops.size(); j++) {
                routePart.to = routeIterator.second.stops[j];
                routePart.weight = routing_settings_.bus_wait_time +
                                   (measureDistanceOnRoute(routeIterator.second, i, j) /
                                    routing_settings_.bus_velocity) / (1000.0 / 60.0);
                size_t EdgeId = routesWeights->AddEdge(routePart);
                if (EdgeId >= EdgeIds.size())
                    EdgeIds.resize(EdgeId + 1);
                EdgeIds[EdgeId] = {&routeIterator.second, i, j};
            }
        }
    }
    routesRouter.emplace(Graph::Router<WEIGHTSPRECISION>(*routesWeights));
}

BusDB::PathPart BusDB::toPathPart(Graph::EdgeId edge_id) const {
  BusDB::PathPart result;
  const Graph::Edge<WEIGHTSPRECISION>& edge = routesWeights->GetEdge(edge_id);
  const EdgeInfo& edgeInfo = EdgeIds.at(edge_id);
  result.routeNumber = edgeInfo.route->number;
  result.stopName = stopsIds.at(edgeInfo.route->stops.at(edgeInfo.from))->first;
  result.stopSpan = edgeInfo.to - edgeInfo.from;
  result.bus_wait_time = routing_settings_.bus_wait_time;
  result.bus_ride_time = edge.weight - routing_settings_.bus_wait_time;
  return result;
}

optional<vector<BusDB::PathPart>> BusDB::ComputeFromTo(string fromStopName, string toStopName) const {
  vector<BusDB::PathPart> result;
  if (fromStopName == toStopName) return { move(result) };
  optional<Graph::Router<WEIGHTSPRECISION>::RouteInfo> iternalResult = routesRouter->BuildRoute(getVertexId(&*allStops.find(fromStopName)),
                                                                                                getVertexId(&*allStops.find(toStopName)));
  if (!iternalResult) return {};
  result.reserve(iternalResult->edge_count);
  for (size_t i=0; i < iternalResult->edge_count; i++) {
    result.push_back(toPathPart(
				routesRouter->GetRouteEdge(iternalResult->id,i)));
  }
  routesRouter->ReleaseRoute(iternalResult->id);
  return result;
}

// void BusDB::AddGeneric(BusDB::DataWrapper data) {
//   switch (data.type_) {
//   case BusDB::DataType::STOP :
//     BusDB::AddStopInfo(data.data);
//     break;
//   case BusDB::DataType::ROUTE :
//     BusDB::AddRoute(data.data);
//     break;
//   case BusDB::DataType::EMPTY :
//     throw runtime_error("DataType::EMPTY");
//     break;
//   default:
//     throw runtime_error("DataType unknown");
//   }
//}
