#include "dbjson.h"

#include <unordered_set>
#include <cassert>

using namespace std;

enum class QueryType {
    STOP,
    ROUTE,
    FromTo
};

QueryType getNodeType(const Json::Node& node) {
    const string_view type = node.AsMap().at("type").AsString();
    if (type == "Stop") {
        return QueryType::STOP;
    } else if (type == "Bus") {
        return QueryType::ROUTE;
    } else if (type == "Route") {
        return QueryType::FromTo;
    } else {
        throw runtime_error("Unknown data type: " + string(type));
    }
}


Stop StopFromJSON(const Json::Node& stop_node) {
    const map<string, Json::Node>& stopJson = stop_node.AsMap();
    Stop result;
    result.location.latitude = stopJson.at("latitude").AsDouble();
    result.location.longitude = stopJson.at("longitude").AsDouble();
    result.name = stopJson.at("name").AsString();
    for (const auto& [name, distance]: stopJson.at("road_distances").AsMap()) {
        result.measuredDistances[name] = distance.AsInt();
    }
    return result;
}

Route RouteFromJSON(const Json::Node& route_node) {
    const map<string, Json::Node>& routeJson = route_node.AsMap();
    Route result;
    result.number = routeJson.at("name").AsString();

    unordered_set<string_view> unique_counter;
    result.stops.reserve(routeJson.at("stops").AsArray().size());
    for (const Json::Node& node: routeJson.at("stops").AsArray()) {
        result.stops.push_back(node.AsString());
        unique_counter.insert(node.AsString());
    }
    result.uniqueStops = unique_counter.size();
    if (!routeJson.at("is_roundtrip").AsBool()) {
        result.stops.reserve(result.stops.size() * 2 - 1);
        for (size_t i = result.stops.size(); i >= 2; i--) {
            result.stops.push_back(result.stops[i - 2]);
        }
    }

    return result;
}

RoutingSettings RoutingSettingsFromJSON(const Json::Node& routing_settings_node) {
    const map<string, Json::Node>& RoutingSettingsJson = routing_settings_node.AsMap();
    return {.bus_wait_time = static_cast<uint>(RoutingSettingsJson.at("bus_wait_time").AsInt()),
            .bus_velocity = RoutingSettingsJson.at("bus_velocity").AsDouble()};
}

void dbConvertAndAdd(const Json::Node& request, BusDB& db) {
    switch (getNodeType(request)) {
        case QueryType::STOP :
            db.AddStopInfo(StopFromJSON(request));
            break;
        case QueryType::ROUTE :
            db.AddRoute(RouteFromJSON(request));
            break;
        default:
            throw runtime_error("Unsuported JSON node type");
    }
}

Json::Node stopInfoJSON(int request_id, optional<set<string_view>> routesForStop) {
    map<string, Json::Node> answer;
    answer["request_id"] = {request_id};
    if (!routesForStop) {
        answer["error_message"] = {string("not found")};
        return {move(answer)};
    }
    vector<Json::Node> buses;
    buses.reserve(routesForStop->size());
    for (string_view route: routesForStop.value()) {
        buses.emplace_back(string(route));
    }
    answer["buses"] = move(buses);
    return {move(answer)};
}

Json::Node routeInfoJSON(int request_id, optional<Route> route) {
    map<string, Json::Node> answer;
    answer["request_id"] = {request_id};
    if (!route) {
        answer["error_message"] = {string("not found")};
        return {move(answer)};
    }
    answer["route_length"] = {static_cast<int>(route->measuredDistance.value())};
    answer["curvature"] = {route->measuredDistance.value() / route->geographicalDistance.value()};
    answer["stop_count"] = {static_cast<int>(route->stopsOnRoute())};
    answer["unique_stop_count"] = {static_cast<int>(route->uniqueStops)};
    return {move(answer)};
}

Json::Node ShortestPathJSON(int request_id, optional<vector<BusDB::PathPart>> path) {
    map<string, Json::Node> answer;
    answer["request_id"] = {request_id};
    if (!path) {
        answer["error_message"] = {string("not found")};
        return {move(answer)};
    }
    vector<Json::Node> items;
    double total_time = 0;
    items.reserve(path->size());
    for (const BusDB::PathPart& part: path.value()) {
        total_time += part.bus_wait_time + part.bus_ride_time;
        map<string, Json::Node> temp;
        //Bus wait
        temp["time"] = {static_cast<int>(part.bus_wait_time)};
        temp["type"] = {string("Wait")};
        temp["stop_name"] = {string(part.stopName)};
        items.emplace_back(move(temp));
        //Bus ride
        temp["span_count"] = {static_cast<int>(part.stopSpan)};
        temp["bus"] = {string(part.routeNumber)};
        temp["time"] = {static_cast<double>(part.bus_ride_time)};
        temp["type"] = {string("Bus")};
        items.emplace_back(move(temp));
    }
    answer["items"] = {move(items)};
    answer["total_time"] = {total_time};
    return {move(answer)};
}

Json::Node ProcessRequest(const Json::Node& request, const BusDB& db) {
    int request_id = request.AsMap().at("id").AsInt();
    switch (getNodeType(request)) {
        case QueryType::FromTo :
            return ShortestPathJSON(request_id,
                                    db.ComputeFromTo(request.AsMap().at("from").AsString(),
                                                     request.AsMap().at("to").AsString()));
        case QueryType::STOP :
            return stopInfoJSON(request_id,
                                db.RoutesForStop(request.AsMap().at("name").AsString()));
        case QueryType::ROUTE :
            return routeInfoJSON(request_id,
                                 db.FindRoute(request.AsMap().at("name").AsString()));
        default:
            throw runtime_error("Unsuported JSON node type");
    }
}

Json::Document ProcessRequests(const Json::Node& stat_requests, const BusDB& db) {
    vector<Json::Node> answers;
    answers.reserve(stat_requests.AsArray().size());
    for (const Json::Node& request: stat_requests.AsArray()) {
        answers.push_back(ProcessRequest(request, db));
    }
    return Json::Document(std::move(answers));
}

BusDB DBfromJSON(const Json::Node& input) {
    BusDB db;
    for (const Json::Node& request: input.AsMap().at("base_requests").AsArray()) {
        dbConvertAndAdd(request, db);
    }

    db.SetRoutingSettings(RoutingSettingsFromJSON(input.AsMap().at("routing_settings")));
    db.PrepareRoutesRouter();

    return db;
}
