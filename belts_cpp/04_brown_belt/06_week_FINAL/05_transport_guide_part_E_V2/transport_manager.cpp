#include "transport_manager.h"
#include "bus.h"
#include "graph.h"
#include "stop_manager.h"
#include "transport_manager_command.h"

#include <iterator>
#include <sstream>
#include <set>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

void TransportManager::InitStop(const string& name) {
  if (!_stop_name_to_idx.count(name) || (_stop_name_to_idx.count(name) && _stops[_stop_name_to_idx[name]].Name() != name)) {
    _stops.emplace_back(name);
      _stop_name_to_idx[name] = _stops.size() - 1;
  }
}

void TransportManager::AddStop(const string& current_stop_name,
                               double latitude,
                               double longitude,
                               const unordered_map<string, unsigned int>& distances) {
  InitStop(current_stop_name);

  size_t id = _stop_name_to_idx[current_stop_name];
  _stops[id].SetCoordinates(Coordinates{latitude, longitude});

    for (const auto& [next_stop_name, dist]: distances) {
        InitStop(next_stop_name);
        _distances[id][_stop_name_to_idx[next_stop_name]] = dist;

        if (!_distances[_stop_name_to_idx[next_stop_name]].count(id) ||
            (_distances[_stop_name_to_idx[next_stop_name]].count(id) && _distances[_stop_name_to_idx[next_stop_name]][id] == 0)) {
            _distances[_stop_name_to_idx[next_stop_name]][id] = dist;
        }
    }
}

void TransportManager::AddBus(const RouteNumber& bus_no,
                              const std::vector<std::string>& stop_names,
                              bool is_roundtrip) {
    _buses[string{bus_no}] = is_roundtrip ? BusRoute::CreateRoundtripBusRoute(bus_no, stop_names)
                                          : BusRoute::CreateOrdinaryBusRoute(bus_no, stop_names);
}

std::pair<unsigned int, double> TransportManager::ComputeBusRouteLength(const RouteNumber& route_number) {
  if (!_buses.count(route_number)) {
    return {0, 0};
  }

  if (auto route_length = _buses[route_number].RouteLength(); route_length.has_value()) {
    return route_length.value();
  }

  unsigned int distance_road{0};
  double distance_direct{0.0};
  vector<const Stop*> bus_stops;
  for (const auto& stop_name : _buses[route_number].Stops()) {
    InitStop(stop_name);
    bus_stops.push_back(&_stops[_stop_name_to_idx[stop_name]]);
  }

  for (size_t i = 0; i < bus_stops.size() - 1; ++i) {
    distance_direct += Coordinates::Distance(bus_stops[i]->StopCoordinates(),
                                             bus_stops[i + 1]->StopCoordinates());
    distance_road += _distances[_stop_name_to_idx[bus_stops[i]->Name()]][_stop_name_to_idx[bus_stops[i + 1]->Name()]];
  }

  _buses[route_number].SetRouteLength(distance_road, distance_direct);
  return {distance_road, distance_direct};
}

StopInfo TransportManager::GetStopInfo(const string& stop_name, size_t request_id) {
  if (!_stop_name_to_idx.count(stop_name)) {
    return StopInfo{
      .request_id = request_id,
      .error_message = "not found",
    };
  }

  set<RouteNumber> buses_with_stop;
  for (const auto& bus : _buses) {
    if (bus.second.ContainsStop(stop_name)) {
        buses_with_stop.insert(bus.first);
    }
  }

  return StopInfo{
    .buses = vector<string>{begin(buses_with_stop), end(buses_with_stop)},
    .request_id = request_id,
  };
}

BusInfo TransportManager::GetBusInfo(const RouteNumber& bus_no, size_t request_id) {
  if (!_buses.count(bus_no)) {
    return BusInfo{
      .request_id = request_id,
      .error_message = "not found",
    };
  }

  const auto& bus = _buses.at(bus_no);
  const auto [road_length, direct_length] = ComputeBusRouteLength(bus_no);

  return BusInfo {
    .route_length = road_length,
    .request_id = request_id,
    .curvature = road_length / direct_length,
    .stop_count = bus.Stops().size(),
    .unique_stop_count = bus.UniqueStopNumber(),
  };
}

void TransportManager::CreateRoutes() {
    _road_graph = make_unique<Graph::DirectedWeightedGraph<double>>(2 * _stops.size());

  for (size_t i = 0; i < _stops.size(); ++i) {
      _road_graph->AddEdge(Graph::Edge<double>{
              .from = 2 * i,
              .to = 2 * i + 1,
              .weight = static_cast<double>(_routing_settings.bus_wait_time),
      });

    _edge_description.emplace_back(WaitActivity{
      .type = "Wait",
      .time = _routing_settings.bus_wait_time,
      .stop_name = _stops[i].Name(),
    });
  }

  for (const auto& [bus_no, bus_route] : _buses) {
    const vector<string>& bus_stop_names = bus_route.Stops();

    for (size_t i = 0; i < bus_stop_names.size(); ++i) {
      double time_sum{0.0};
      unsigned int span_count{0};

      for (size_t j = i + 1; j < bus_stop_names.size(); ++j) {
        time_sum += _distances[_stop_name_to_idx[bus_stop_names[j - 1]]][_stop_name_to_idx[bus_stop_names[j]]] / (_routing_settings.bus_velocity * 1000 / 60);

        _road_graph->AddEdge(Graph::Edge<double>{
            .from = 2 * _stop_name_to_idx[bus_stop_names[i]] + 1,
            .to = 2 * _stop_name_to_idx[bus_stop_names[j]],
            .weight = time_sum
        });

        _edge_description.emplace_back(BusActivity{
          .type = "Bus",
          .time = time_sum,
          .bus = bus_no,
          .span_count = ++span_count,
        });
      }
    }
  }

    _router = make_unique<Graph::Router<double>>(*_road_graph);
}

  RouteInfo TransportManager::GetRouteInfo(const std::string& from,
                                           const std::string& to,
                                           size_t request_id) {
    size_t from_id = 2 * _stop_name_to_idx[from];
    size_t to_id = 2 * _stop_name_to_idx[to];
    auto route_info = _router->BuildRoute(from_id, to_id);

    if (!route_info.has_value()) {
      return {
        .request_id = request_id,
        .error_message = "not found",
      };
    }

    std::vector<std::variant<WaitActivity, BusActivity>> items;
    auto id = route_info.value().id;
    for (size_t i = 0; i < route_info.value().edge_count; ++i) {
      auto edge_id = _router->GetRouteEdge(id, i);
      items.push_back(_edge_description[edge_id]);
    }

    return {
      .request_id = request_id,
      .total_time = route_info.value().weight,
      .items = items,
    };
  }
