#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <memory>  // std::hash
#include <numeric> // std::accumulate
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility> // std::move, std::pair
#include <variant>
#include <vector>

using namespace std;

// ====================================================================================================================
namespace Json {

    class Node : std::variant<
            std::string,
            std::vector<Node>,
            std::map<std::string, Node>,
            int,
            double,
            bool> {
      public:
        using variant::variant;

        const auto& AsArray() const {
            return std::get<std::vector<Node>>(*this);
        }

        const auto& AsMap() const {
            return std::get<std::map<std::string, Node>>(*this);
        }

        int AsInt() const {
            return std::get<int>(*this);
        }

        const auto& AsString() const {
            return std::get<std::string>(*this);
        }

        double AsDouble() const {
            double res;
            try {
                res = std::get<double>(*this);
            } catch (std::bad_variant_access&) {
                res = static_cast<double>(std::get<int>(*this));
            }
            return res;
        }

        bool AsBool() const {
            return std::get<bool>(*this);
        }
    };

    class Document {
      public:
        explicit Document(Node root);

        const Node& GetRoot() const;

      private:
        Node root;
    };

    Document Load(std::istream& input);
}  // namespace Json
// ====================================================================================================================
namespace Json {

    Document::Document(Node root)
            : root(std::move(root)) { }

    const Node& Document::GetRoot() const {
        return root;
    }

    Node LoadNode(istream& input);  // forward declaration

    Node LoadArray(istream& input) {
        vector<Node> result;

        for (char c; input >> c && c != ']';) {
            if (c != ',') {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }

        return Node{std::move(result)};
    }

    Node LoadString(istream& input) {
        string line;
        getline(input, line, '"');
        return Node{std::move(line)};
    }

    Node LoadDict(istream& input) {
        map<string, Node> result;

        for (char c; input >> c && c != '}';) {
            if (c == ',') {
                input >> c;
            }

            string key = LoadString(input).AsString();
            input >> c;
            result.emplace(std::move(key), LoadNode(input));
        }

        return Node{std::move(result)};
    }

    Node LoadVal(istream& input) {
        string str;
        char c;
        input >> c;
        while (isdigit(c) || c == '-' || c == '.') {
            str += c;
            input >> c;
        }
        input.putback(c);

        if (str.find('.') == string::npos) {
            return Node{stoi(str)};
        }

        return Node{stod(str)};
    }

    Node LoadBool(istream& input) {
        string str;
        char c;
        input >> c;
        while (isalpha(c)) {
            str += c;
            input >> c;
        }

        if (c == ',' || c == '}') {
            input.putback(c);
        }

        if (str == "false") {
            return Node{false};
        }

        return Node{true};
    }

    Node LoadNode(istream& input) {
        char c;
        input >> c;

        if (c == '[') {
            return LoadArray(input);
        } else if (c == '{') {
            return LoadDict(input);
        } else if (c == '"') {
            return LoadString(input);
        } else if (isalpha(c)) {
            input.putback(c);
            return LoadBool(input);
        } else {
            input.putback(c);
            return LoadVal(input);
        }
    }

    Document Load(istream& input) {
        return Document{LoadNode(input)};
    }
}  // namespace Json
// ====================================================================================================================
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

struct BusRouteStats {  // statistics for a given bus route
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

    enum class RetrieveCommand : char {
        Bus,
        Stop
    };

    std::unordered_map<std::string_view, RetrieveCommand> str_to_retrieve_command = {
            {"Bus",  RetrieveCommand::Bus},
            {"Stop", RetrieveCommand::Stop}
    };

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
// ====================================================================================================================

// ====================================================================================================================
pair<Stop, optional<NextStopsToDistancesMap>>
TransportGuide::GetStopFromJsonMap(const map<string, Json::Node>& request_as_json_map) {
    auto [it_to_current_stop_name, is_inserted1] = _stop_names.insert(request_as_json_map.at("name").AsString());
    Stop stop{*it_to_current_stop_name,
              request_as_json_map.at("latitude").AsDouble(),
              request_as_json_map.at("longitude").AsDouble()};

    if (!request_as_json_map.count("road_distances")
        || request_as_json_map.at("road_distances").AsMap().empty()) {
        return {stop, nullopt};
    } else {
        NextStopsToDistancesMap next_stops_to_distances;
        const auto& distances_map = request_as_json_map.at("road_distances").AsMap();

        for (const auto& [name_of_next_stop, distance_as_json_node]: distances_map) {
            auto [it_to_next_stop_name, is_inserted2] = _stop_names.insert(name_of_next_stop);
            next_stops_to_distances.emplace(*it_to_next_stop_name, distance_as_json_node.AsInt());
        }
        return {stop, std::move(next_stops_to_distances)};
    }
}

pair<string_view, vector<string_view>>
TransportGuide::GetBusRouteFromJsonMap(const map<string, Json::Node>& request_as_json_map) {
    vector<string_view> stop_names_on_bus_route;
    string_view bus_name = *_bus_names.insert(request_as_json_map.at("name").AsString()).first;
    vector<Json::Node> stop_names_as_json_nodes = request_as_json_map.at("stops").AsArray();

    bool is_roundtrip = request_as_json_map.at("is_roundtrip").AsBool();
    stop_names_on_bus_route.reserve(is_roundtrip
                                    ? stop_names_as_json_nodes.size()
                                    : stop_names_as_json_nodes.size() * 2 - 1);

    for (const auto& json_stop_name: stop_names_as_json_nodes) {
        auto [it_stop_name_on_route, is_inserted] = _stop_names.insert(json_stop_name.AsString());
        stop_names_on_bus_route.push_back(*it_stop_name_on_route);
    }

    if (!is_roundtrip) {
        for (auto it = next(stop_names_on_bus_route.rbegin()); it != stop_names_on_bus_route.rend(); ++it) {
            stop_names_on_bus_route.push_back(*it);
        }
    }

    return {bus_name, std::move(stop_names_on_bus_route)};
}

void TransportGuide::AddStopsAndPreprocessBusRoutes(const vector<Json::Node>& create_requests) {
    for (const auto& request_as_JSON_map_node: create_requests) {
        const map<string, Json::Node>& request_as_map = request_as_JSON_map_node.AsMap();
        const string& request_type = request_as_map.at("type").AsString();

        // запросы на добавление остановок обрабатываем сразу;
        if (request_type == "Stop") {
            auto [current_stop, next_stop_names_to_distances] = GetStopFromJsonMap(request_as_map);
            _stops.push_back(current_stop);
            Stop& fixed_stop = _stops.back();
            _stop_name_to_stop_ptr.insert({fixed_stop.stop_name_view, &(fixed_stop)});

            if (next_stop_names_to_distances.has_value()) {
                // ПРЕДВАРИТЕЛЬНОЕ заполнение таблицы расстояний м/д остановками, т.к. в данный момент информация
                // о 2-ой остановке скорее всего недоступна - ещё не было запроса на добавление остановки "next_stop"
                for (auto [next_stop_name_view, distance]: *next_stop_names_to_distances) {
                    _pair_stop_names_view_to_distance.insert({{fixed_stop.stop_name_view, next_stop_name_view},
                                                              distance});
                }
            }
        } else if (request_type == "Bus") {
            // сохраняем информацию о запросах на создание автобусных маршрутов во временном хранилище
            _bus_name_to_stop_names.insert(GetBusRouteFromJsonMap(request_as_map));
        } else {
            throw std::logic_error("Unsupported request: \"" + request_type + "\"");
        }
    }
}

double TransportGuide::CalculateGeoDistanceBetweenTwoStops(Stop* stop_1, Stop* stop_2) {
    return acos(sin(stop_1->latitude_rad) * sin(stop_2->latitude_rad) +
                cos(stop_1->latitude_rad) * cos(stop_2->latitude_rad) *
                cos(abs(stop_1->longitude_rad - stop_2->longitude_rad)))
           * 6'371'000;
}

void TransportGuide::CalculateBusRouteStatistics(string_view bus_name,
                                                 BusRoute& bus_route,
                                                 const vector<string_view>& stop_names,
                                                 const unordered_set<string_view>& unique_stops) {
    vector<double> geo_distances;
    geo_distances.reserve(stop_names.size() - 1);

    vector<unsigned long> road_distances;
    road_distances.reserve(stop_names.size() - 1);

    for (size_t i = 0, j = 1; j < stop_names.size(); ++i, ++j) {
        Stop* stop1_ptr = bus_route[i];
        Stop* stop2_ptr = bus_route[j];
        geo_distances.push_back(CalculateGeoDistanceBetweenTwoStops(stop1_ptr, stop2_ptr));

        if (!_distances_table.empty()) {
            auto it = _distances_table.find(pair{stop1_ptr, stop2_ptr});
            if (it != _distances_table.end()) {
                road_distances.push_back(it->second);
            } else {
                ostringstream oss;
                oss << "Distance for pair of stops: {"
                    << stop1_ptr->stop_name_view << ", " << stop2_ptr->stop_name_view << "} not found."
                    << endl;
                throw std::logic_error(oss.str());
            }
        }
    }
    // =============================================
    double length_by_coordinates = accumulate(geo_distances.begin(),
                                              geo_distances.end(),
                                              0.0);
    unsigned long length_by_roads = accumulate(road_distances.begin(),
                                               road_distances.end(),
                                               0ul);
    double curvature = static_cast<double>(length_by_roads) / length_by_coordinates;

    _bus_name_to_bus_route_stats[bus_name] = BusRouteStats{
            stop_names.size(),
            unique_stops.size(),
            length_by_coordinates,
            length_by_roads,
            curvature
    };
}

void TransportGuide::FinallyProcessBusRoutes() {
    for (auto& [bus_name_view, stop_names]: _bus_name_to_stop_names) {
        _bus_routes.emplace_back(stop_names.size());
        BusRoute& current_bus_route = _bus_routes.back();
        _bus_name_to_bus_route[bus_name_view] = &current_bus_route;

        unordered_set<string_view> unique_stops;
        for (size_t i = 0; i < stop_names.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stop_names[i]];
            unique_stops.insert(stop_names[i]);
            _stop_name_to_bus_routes[stop_names[i]].insert(bus_name_view);
        }

        CalculateBusRouteStatistics(bus_name_view, current_bus_route, stop_names, unique_stops);
    }
}

void TransportGuide::CreateDistancesTable() {
    for (auto [pair_stop_names_view, distance]: _pair_stop_names_view_to_distance) {
        string_view current_stop_name = pair_stop_names_view.first;
        string_view next_stop_name = pair_stop_names_view.second;
        Stop* current_stop_ptr = nullptr;
        Stop* next_stop_ptr = nullptr;

        if (auto it_current = _stop_name_to_stop_ptr.find(current_stop_name);
                it_current != _stop_name_to_stop_ptr.end()) {
            current_stop_name = it_current->first;
            current_stop_ptr = it_current->second;
        }

        if (auto it_next = _stop_name_to_stop_ptr.find(next_stop_name);
                it_next != _stop_name_to_stop_ptr.end()) {
            next_stop_name = it_next->first;
            next_stop_ptr = it_next->second;
        }

        if (!(current_stop_ptr && next_stop_ptr)) {
            throw std::logic_error("Distance between "
                                   + string(current_stop_name) + " and " + string(next_stop_name) +
                                   " can not be calculated\n");
        }

        _distances_table.insert({{current_stop_ptr, next_stop_ptr}, distance});
        if (!_pair_stop_names_view_to_distance.count({next_stop_name, current_stop_name})) {
            _distances_table.insert({{next_stop_ptr, current_stop_ptr}, distance});
        }
    }
}

void TransportGuide::CreateDataBaseFromJSON(const vector<Json::Node>& base_requests) {
    AddStopsAndPreprocessBusRoutes(base_requests);
    CreateDistancesTable();
    FinallyProcessBusRoutes();
}
// ====================================================================================================================
void TransportGuide::PrintBusStatsIntoJSON(const map<string, Json::Node>& request_map,
                                           ostream& out_stream ) const {
    string_view bus_name = request_map.at("name").AsString();
    int request_id = request_map.at("id").AsInt();

    if (const auto& bus_stats_opt = GetStatsForBusRoute(bus_name);
            bus_stats_opt.has_value()) {
        const auto& bus_stats = *bus_stats_opt;
        out_stream << R"(    "route_length": )" << bus_stats.get().length_by_roads << "," << '\n'
                   << R"(    "request_id": )" << request_id << "," << '\n'
                   << R"(    "curvature": )" << bus_stats.get().curvature << "," << '\n'
                   << R"(    "stop_count": )" << bus_stats.get().stops_count << "," << '\n'
                   << R"(    "unique_stop_count": )" << bus_stats.get().unique_stops_count << '\n';
    } else {
        out_stream << R"(    "request_id": )" << request_id << "," << '\n'
                   << R"(    "error_message": "not found")" << '\n';
    }
}

void TransportGuide::PrintStopStatsIntoJSON(const map<string, Json::Node>& request_map,
                                            ostream& out_stream ) const {
    string_view stop_name = request_map.at("name").AsString();
    int request_id = request_map.at("id").AsInt();

    if (_stop_names.count(string(stop_name))) {
        if (_stop_name_to_bus_routes.count(stop_name)) {
            out_stream << R"(    "buses": [)";
            const set<string_view>& bus_names = _stop_name_to_bus_routes.at(stop_name);

            bool is_first_bus_name = true;
            for (auto bus_name: bus_names) {
                if (!is_first_bus_name) {
                    out_stream << ',';
                }
                is_first_bus_name = false;
                out_stream << '\n'
                           << R"(      ")" << bus_name << R"(")";
            }
            out_stream << '\n'
                       << R"(    ],)" << '\n'
                       << R"(    "request_id": )" << request_id << '\n';
        } else {  // остановка с именем "bus_name" существует, но через неё не проходит ни один маршрут
            out_stream << R"(    "buses": [],)" << '\n'
                       << R"(    "request_id": )" << request_id << '\n';
        }
    } else {  // остановка с именем "bus_name" НЕ существует
        out_stream << R"(    "request_id": )" << request_id << "," << '\n'
                   << R"(    "error_message": "not found")" << '\n';
    }
}

void TransportGuide::ProcessRetrieveQueries(const std::vector<Json::Node>& retrieve_requests,
                                            std::ostream& out_stream) const {
    out_stream.precision(6);
    out_stream << "[";

    bool is_first_map = true;
    for (const auto& request: retrieve_requests) {
        const auto& request_map = request.AsMap();
        const string& type = request_map.at("type").AsString();

        if (!is_first_map) {
            out_stream << ',';
        }
        is_first_map = false;
        out_stream << '\n' << "  {" << '\n';

        if (type == "Bus") {
            PrintBusStatsIntoJSON(request_map, out_stream);
        } else if (type == "Stop") {
            PrintStopStatsIntoJSON(request_map, out_stream);
        } else {
            throw std::logic_error(R"(Unsupported request type: ")" + type + R"(" encountered.)");
        }

        out_stream << "  }";
    }

    out_stream << '\n' << "]";
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

optional<reference_wrapper<const BusRouteStats>> TransportGuide::GetStatsForBusRoute(string_view bus_route_name) const {
    if (_bus_name_to_bus_route_stats.count(bus_route_name)) {
        return _bus_name_to_bus_route_stats.at(bus_route_name);
    } else {
        return nullopt;
    }
}

const TransportGuide::DistancesTable& TransportGuide::GetDistancesTable() const {
    return _distances_table;
}
// ====================================================================================================================

int main() {
    Json::Document document = Json::Load(cin);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());
    transport_guide.ProcessRetrieveQueries(root_map.at("stat_requests").AsArray());

    return 0;
}