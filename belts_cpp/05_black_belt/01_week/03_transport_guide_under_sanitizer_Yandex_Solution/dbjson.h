#pragma once

#include "json.h"
#include "busdb.h"

Stop StopFromJSON(const Json::Node& stop_node);

Route RouteFromJSON(const Json::Node& route_node);

RoutingSettings RoutingSettingsFromJSON(const Json::Node& route_settings_node);

BusDB DBfromJSON(const Json::Node& base_requests);

Json::Node RequestToJSON(const Json::Node& request);

Json::Document ProcessRequests(const Json::Node& stat_requests, const BusDB& db);

Json::Node stopInfoJSON(int request_id, std::optional<std::set<std::string_view>> routesForStop);

Json::Node routeInfoJSON(int request_id, std::optional<Route> route);

Json::Node ShortestPathJSON(int request_id, std::optional<std::vector<BusDB::PathPart>> path);
