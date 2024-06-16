#include <algorithm>
#include <cmath>    // std::acos, std::sin
#include <iomanip>  // std::setprecision
#include <iostream>
#include <numeric>  // std::accumulate
#include <unordered_set>

#include "parse.h"
#include "transport_guide.h"
#include "json.h"

using namespace std;

pair<Stop, optional<NextStopsToDistancesMap>>
TransportGuide::GetStopFromJsonMap(const map<string, Json::Node>& json_map) {
    auto [it_to_current_stop_name, is_inserted1] = _stop_names.insert(json_map.at("name").AsString());
    Stop stop{*it_to_current_stop_name,
              json_map.at("latitude").AsDouble(),
              json_map.at("longitude").AsDouble()};

    if (!json_map.count("road_distances") || json_map.at("road_distances").AsMap().empty()) {
        return {stop, nullopt};
    } else {
        NextStopsToDistancesMap next_stops_to_distances;
        const auto& distances_map = json_map.at("road_distances").AsMap();

        for (const auto& [name_of_next_stop, distance_as_json_node]: distances_map) {
            auto [it_to_next_stop_name, is_inserted2] = _stop_names.insert(name_of_next_stop);
            next_stops_to_distances.emplace(*it_to_next_stop_name, distance_as_json_node.AsInt());
        }
        return {stop, std::move(next_stops_to_distances)};
    }
}

pair<string_view, vector<string_view>> TransportGuide::GetBusRouteFromJsonMap(const map<string, Json::Node>& json_map) {
    vector<string_view> stop_names_on_bus_route;
    string_view bus_route_name = *_bus_names.insert(json_map.at("name").AsString()).first;
    vector<Json::Node> stop_names_as_json_nodes = json_map.at("stops").AsArray();

    bool is_roundtrip = json_map.at("is_roundtrip").AsBool();
    stop_names_on_bus_route.reserve(is_roundtrip
                                 ? stop_names_as_json_nodes.size() * 2 - 1
                                 : stop_names_as_json_nodes.size());

    for (const auto& json_stop_name : stop_names_as_json_nodes) {
        auto [it_stop_name_on_route, is_inserted] = _stop_names.insert(json_stop_name.AsString());
        stop_names_on_bus_route.push_back(*it_stop_name_on_route);
    }

    if (is_roundtrip) {
        std::copy_backward(next(stop_names_on_bus_route.rbegin()), stop_names_on_bus_route.rend(),
                           stop_names_on_bus_route.end());
    }

    return {bus_route_name, std::move(stop_names_on_bus_route)};
}

// transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());
void TransportGuide::CreateDataBaseFromJSON(const vector<Json::Node>& create_requests) {
    unordered_map<string_view, vector<string_view>> bus_name_to_stop_names;

    for (const auto& request_as_JSON_map_node : create_requests) {
        const map<string, Json::Node>& request_as_map = request_as_JSON_map_node.AsMap();
        const string& request_type = request_as_map.at("type").AsString();

        // запросы на добавление остановок обрабатываем сразу;
        // запросы на добавление автобусных маршрутов парсим, сохраняем во временном объекте,
        // а обрабатываем только после окончания обработки всего JSON-документа (когда будет информация обо всех остановках)
        if (request_type == "Stop") {
            // TODO: fails here
            auto [current_stop, next_stop_names_to_distances] = GetStopFromJsonMap(request_as_map);
            _stops.push_back(current_stop);
            Stop& fixed_stop = _stops.back();
            _stop_name_to_stop_ptr.insert({fixed_stop.stop_name_view, &(fixed_stop)});

            if (next_stop_names_to_distances.has_value()) {
                // Ниже происходит ПРЕДВАРИТЕЛЬНОЕ заполнение таблицы расстояний между остановками, поскольку
                // в данный момент информация о 2-ой остановке остановочного интервала (next_stop) скорее всего недоступна
                // т.е. еще не было запроса на добавление остановки "next_stop"
                for (auto [next_stop_name_view, distance]: *next_stop_names_to_distances) {
                    _pair_stop_names_view_to_distance.insert({{fixed_stop.stop_name_view, next_stop_name_view},
                                                              distance});
                }
            }
        } else if (request_type == "Bus") {
            // сохраняем информацию о запросах на создание автобусных маршрутов во временном хранилище
//            auto [bus_name_view, stop_names_on_bus_route] = GetBusRouteFromJsonMap(request_as_map);
//            bus_name_to_stop_names.insert({bus_name_view, std::move(stop_names_on_bus_route)});
            bus_name_to_stop_names.insert(GetBusRouteFromJsonMap(request_as_map));
        } else {
            throw std::logic_error("Unsupported request: \"" + request_type + "\"");
        }
    }

    // после того как все json-запросы обработаны, заполняем внутренние структуры данных автобусных маршрутов
    for (auto& [bus_name, stop_names] : bus_name_to_stop_names) {
        _bus_routes.emplace_back(stop_names.size());
        BusRoute& current_bus_route = _bus_routes.back();
        _bus_name_to_bus_route[bus_name] = &current_bus_route;

        unordered_set<string_view> unique_stops;

        for (size_t i = 0; i < stop_names.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stop_names[i]];
            unique_stops.insert(stop_names[i]);
            _stop_name_to_bus_routes[stop_names[i]].insert(bus_name);
        }

        vector<double> geo_distances;
        geo_distances.reserve(stop_names.size() - 1);

        vector<unsigned long> road_distances;
        road_distances.reserve(stop_names.size() - 1);

        for (size_t i = 0, j = 1; j < stop_names.size(); ++i, ++j) {
            Stop* lhs = current_bus_route[i];
            Stop* rhs = current_bus_route[j];

            geo_distances.push_back(acos(sin(lhs->latitude_rad) * sin(rhs->latitude_rad) +
                                         cos(lhs->latitude_rad) * cos(rhs->latitude_rad) *
                                         cos(abs(lhs->longitude_rad - rhs->longitude_rad)))
                                    * 6'371'000);

            if (!_distances_table.empty()) {
                auto it = _distances_table.find({lhs, rhs});
                if (it != _distances_table.end()) {
                    road_distances.push_back(it->second);
                } else {
                    ostringstream oss;
                    oss << "Distance for pair of stops: {"
                        << lhs->stop_name_view << ", " << rhs->stop_name_view << "} not found."
                        << endl;
                    throw std::logic_error(oss.str());
                }
            }
        }
        // ================
        double length_by_coordinates = accumulate(geo_distances.begin(),
                                                  geo_distances.end(),
                                                  0.0);
        unsigned long length_by_roads = accumulate(road_distances.begin(),
                                                   road_distances.end(),
                                                   0ul);

        _bus_name_to_bus_route_stats[bus_name] = BusRouteStats{
                stop_names.size(),
                unique_stops.size(),
                {length_by_coordinates, length_by_roads}
        };
    }
}

// =============================================================================================

void TransportGuide::PrintBusRouteInfo(std::string_view bus_route_name, std::ostream& out) const {
    if (_bus_name_to_bus_route.count(bus_route_name)) {
        const BusRouteStats& bus_stats = GetStatsForBusRoute(bus_route_name);
        out << "Bus " << bus_route_name << ": "
            << bus_stats.stops_count << " stops on route, "
            << bus_stats.unique_stops_count << " unique stops, "
            << std::setprecision(6) << bus_stats.lengths.roads_length << " route length, "
            << std::setprecision(7) << (double(bus_stats.lengths.roads_length) / bus_stats.lengths.geo_length) << " curvature"
            << endl;
    } else {
        out << "Bus " << bus_route_name
            << ": not found"
            << endl;
    }
}

void TransportGuide::PrintStopInfo(std::string_view stop_name, std::ostream& out) const {
    if (0 == _stop_name_to_stop_ptr.count(stop_name)) {
        out << "Stop " << stop_name
            << ": not found"
            << endl;
    } else {
        if (0 == _stop_name_to_bus_routes.count(stop_name)) {
            out << "Stop " << stop_name
                << ": no buses"
                << endl;
        } else {
            out << "Stop " << stop_name
                << ": buses";
            for (const auto bus_name: _stop_name_to_bus_routes.at(stop_name)) {
                out << " " << bus_name;
            }
            out << endl;
        }
    }
}

void TransportGuide::ProcessRetrievalQueries(std::istream& in, std::ostream& out) {
    size_t retrieve_queries_count;
    in >> retrieve_queries_count >> std::ws;

    string line{};
    for (size_t i = 0; i < retrieve_queries_count; ++i) {
        getline(in, line);
        const auto& [command, arg] = SplitIntoTwoPartsView(line, " ");

        switch (str_to_retrieve_command[command]) {
            case RetrieveCommand::Bus:
                PrintBusRouteInfo(arg, out);
                break;
            case RetrieveCommand::Stop: {
                PrintStopInfo(arg, out);
                break;
            }
        }
    }
}

void TransportGuide::CreateDistancesTable() {
    for (auto [stop_names_pair, distance]: _pair_stop_names_view_to_distance) {
        string_view stop_name_from = stop_names_pair.first;
        string_view stop_name_to = stop_names_pair.second;

        auto it_from = _stop_name_to_stop_ptr.find(stop_name_from);
        auto it_to = _stop_name_to_stop_ptr.find(stop_name_to);

        if (it_from != _stop_name_to_stop_ptr.end() &&
            it_to   != _stop_name_to_stop_ptr.end()) {
            _distances_table.insert({{it_from->second, it_to->second}, distance});

            if (0 == _pair_stop_names_view_to_distance.count({it_to->first, it_from->first})) {
                _distances_table.insert({{it_to->second, it_from->second}, distance});
            }
        } else {
            throw std::logic_error("Distance between "
                                   + string(stop_name_from) + " and " + string(stop_name_to) +
                                   " can not be calculated\n");
        }
    }
}

const deque<Stop>& TransportGuide::GetStops() const {
    return _stops;
}

const TransportGuide::StopNameToStopPtr& TransportGuide::GetStopNameToStopPtr() const {
    return _stop_name_to_stop_ptr;
}

const deque<TransportGuide::BusRoute>& TransportGuide::GetBusRoutes() const {
    return _bus_routes;
}

const TransportGuide::BusNameToBusRoute& TransportGuide::GetBusNameToBusRouteMapping() const {
    return _bus_name_to_bus_route;
}

size_t TransportGuide::GetUniqueStopsCountForBusRoute(string_view bus_route_name) const {
    return _bus_name_to_bus_route_stats.at(bus_route_name).unique_stops_count;
}

const BusRouteStats& TransportGuide::GetStatsForBusRoute(string_view bus_route_name) const {
    return _bus_name_to_bus_route_stats.at(bus_route_name);
}

const TransportGuide::DistancesTable& TransportGuide::GetDistancesTable() const {
    return _distances_table;
}


