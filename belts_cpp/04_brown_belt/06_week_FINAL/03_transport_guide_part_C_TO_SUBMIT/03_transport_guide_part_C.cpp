#include <algorithm>
#include <cmath>
#include <deque>
#include <iomanip> // std::setprecision
#include <iostream>
#include <numeric> // std::accumulate
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility> // std::pair
#include <vector>

using namespace std;

// =================== Forward declarations ===================
struct Stop;
std::string_view ltrim_view(std::string_view s);
std::string_view rtrim_view(std::string_view s);
std::string_view trim_view(std::string_view s);

std::pair<std::string_view, std::string_view> SplitIntoTwoPartsView(std::string_view s, const std::string& sep);
std::vector<std::string_view> SplitBy(std::string_view s, const std::string& sep);
// =========================================================

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

// ============================================================================================================================
void TransportGuide::AddStops(std::istream& in) {
    size_t create_queries_count;
    in >> create_queries_count >> std::ws;
    _buffer.reserve(create_queries_count);

    // полностью обрабатываем ТОЛЬКО ту часть входного потока, которая касается остановок
    string line{};
    for (size_t i = 0; i < create_queries_count; ++i) {
        getline(in, line);
        ProcessCreateQuery(line);
    }
}

void TransportGuide::AddBusRoutes() {
    if (_buffer.empty()) { return; }

    for (string_view bus_route_entry: _buffer) {
        const auto& [bus_route_name, stop_names] = ParseBusRouteView(bus_route_entry);
        _bus_routes.emplace_back(stop_names.size());
        BusRoute& current_bus_route = _bus_routes.back();
        _bus_name_to_bus_route[bus_route_name] = &current_bus_route;

        unordered_set<string_view> unique_stops;

        for (size_t i = 0; i < stop_names.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stop_names[i]];
            unique_stops.insert(stop_names[i]);
            _stop_name_to_bus_routes[stop_names[i]].insert(bus_route_name);
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
                        << lhs->stop_name << ", " << rhs->stop_name << "} not found."
                        << endl;
                    throw std::logic_error(oss.str());
                }
            }
        }

        double length_by_coordinates = accumulate(geo_distances.begin(),
                                                  geo_distances.end(),
                                                  0.0);
        unsigned long length_by_roads = accumulate(road_distances.begin(),
                                                   road_distances.end(),
                                                   0ul);

        _bus_name_to_bus_route_stats[bus_route_name] = BusRouteStats{
                stop_names.size(),
                unique_stops.size(),
               {length_by_coordinates, length_by_roads}
        };
    }
}

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

const deque<Stop>& TransportGuide::GetStops() const {
    return _stops;
}

const vector<string>& TransportGuide::GetBuffer() const {
    return _buffer;
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

// parse string in the following of 2 formats:
// 1) "stop_name: latitude_deg, longitude_deg"
//     Tolstopaltsevo: 55.611087, 37.20829
// 2) "stop_name: latitude_deg, longitude_deg DISTANCEm to other_stop_name"
pair<Stop, optional<TransportGuide::EndStopsToDistances>> TransportGuide::ParseAddStopQuery(string_view s) {
    auto [stop_name_from, rest_part] = SplitIntoTwoPartsView(s, ":");
    vector<string_view> separated_by_comma = SplitBy(rest_part, ",");
    string_view latitude = separated_by_comma[0];
    string_view longitude = separated_by_comma[1];

    if (separated_by_comma.size() == 2u) {  // stop contains just "latitude" and "longitude"
        return {Stop{string(stop_name_from),
                     stod(string{latitude}),
                     stod(string{longitude})},
                nullopt};
    } else if (separated_by_comma.size() > 2u) {
        EndStopsToDistances end_stops_to_distances;

        for (size_t i = 2; i < separated_by_comma.size(); ++i) {
            auto [suffixed_distance, stop_name_to] = SplitIntoTwoPartsView(separated_by_comma[i], string("to"));
            if (auto it1 = _stop_name_to_stop_ptr.find(stop_name_to);
                    it1 != _stop_name_to_stop_ptr.end()) {
                stop_name_to = it1->first;
            } else {
                auto [it2, is_inserted] = _stop_names.insert(string(stop_name_to));
                stop_name_to = *it2;
            }

            suffixed_distance.remove_suffix(1);  // remove letter 'm' at the end of distance
            auto distance = static_cast<size_t>(std::stoi(string(suffixed_distance)));

            end_stops_to_distances.insert({stop_name_to, distance});
        }

        return {Stop{string(stop_name_from),
                     stod(string{latitude}),
                     stod(string{longitude})},
                std::move(end_stops_to_distances)};
    } else {
        throw std::logic_error("Wrong format for adding stop: " + string(stop_name_from));
    }
}

void TransportGuide::ProcessCreateQuery(std::string_view s) {
    s = trim_view(s);
    size_t first_space_pos = s.find_first_of(' ');
    string_view command = s.substr(0, first_space_pos);
    s.remove_prefix(first_space_pos);
    s = trim_view(s);

    switch (str_to_create_command[command]) {
        case CreateCommand::Bus:
            // складываем во временный буфер предобработанные запросы на создание маршрута (без слова "Bus")
            _buffer.emplace_back(s);
            break;
        case CreateCommand::Stop: {
            const auto& [stop_from, end_stop_names_to_distances] = ParseAddStopQuery(s);
            _stops.push_back(stop_from);
            _stop_name_to_stop_ptr.insert({_stops.back().stop_name, &(_stops.back())});

            // Ниже происходит ПРЕДВАРИТЕЛЬНОЕ заполнение таблицы расстояний между остановками, поскольку
            // в данный момент информация о второй остановке остановочного интервала (stop_to) скорее всего недоступна
            // т.е. еще не было запроса на добавление остановки "stop_to"
            if (end_stop_names_to_distances) {
                for (auto [stop_name_to, distance]: *end_stop_names_to_distances) {
                    _pair_stop_names_view_to_distance.insert({{_stops.back().stop_name, stop_name_to}, distance});
                }
            }
            break;
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

pair<string_view, vector<string_view>> TransportGuide::ParseBusRouteView(std::string_view bus_route_entry) {
    auto [bus_route_name, delimited_stops] = SplitIntoTwoPartsView(bus_route_entry, ":");
    delimited_stops = trim_view(delimited_stops);

    size_t first_delim_pos = delimited_stops.find_first_of(">-");
    if (first_delim_pos == string_view::npos) { return {}; }

    const string separator = string(1, delimited_stops[first_delim_pos]);
    vector<string_view> stops = SplitBy(delimited_stops, separator);

    // in case of circular bus route: A - B - C <=> A > B > |C| > B > A
    if (separator == "-") {
        stops.resize(stops.size() * 2 - 1);
        std::copy_backward(next(stops.rbegin()), stops.rend(),
                           stops.end());
    }

    return {bus_route_name, stops};
}

void TransportGuide::ProcessCreationQueries(std::istream& in) {
    AddStops(in);
    CreateDistancesTable();
    AddBusRoutes();
}

// ==============================================================================================================
string_view ltrim_view(string_view s) {
    s.remove_prefix(std::min(s.find_first_not_of(' '), s.size()));
    return s;
}

std::string_view rtrim_view(std::string_view s) {
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(' ') - 1, s.size()));
    return s;
}

std::string_view trim_view(std::string_view s) {
    return ltrim_view(rtrim_view(s));
}

vector<string_view> SplitBy(string_view s, const string& sep) {
    vector<string_view> result;

    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(trim_view(s.substr(0, pos)));
        s.remove_prefix(pos != string_view::npos
                        ? pos + sep.size()
                        : s.size());
    }

    return result;
}

pair<string_view, string_view> SplitIntoTwoPartsView(string_view s, const string& sep) {
    size_t pos = s.find(sep);
    if (pos != string_view::npos) {
        return {trim_view(s.substr(0, pos)),
                trim_view(s.substr(pos + sep.size()))};
    } else {
        return {};
    }
}


int main() {
    TransportGuide guide;
    guide.ProcessCreationQueries(cin);
    guide.ProcessRetrievalQueries(cin);

    return 0;
}
