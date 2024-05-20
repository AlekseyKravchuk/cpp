#include <algorithm>
#include <cmath>
#include <deque>
#include <iomanip> // std::setprecision
#include <iostream>
#include <numeric> // std::accumulate
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
pair<string_view, string_view> SplitIntoTwoPartsView(string_view s, char sep);
std::string_view trim_view(std::string_view s);
Stop ParseStopView(string_view s);
vector<string_view> SplitBy(string_view s, char sep);
// =========================================================

struct Stop {
    std::string stop_name;      // название остановки
    double latitude_deg;        // широта в градусах
    double longitude_deg;       // долгота в градусах

    double latitude_rad;        // широта в радианах
    double longitude_rad;       // долгота в радианах

    Stop(std::string stop_name_, double latitude_deg_, double longitude_deg_)
            : stop_name(std::move(stop_name_)),
              latitude_deg(latitude_deg_),
              longitude_deg(longitude_deg_) {
        latitude_rad = latitude_deg * 3.1415926535 / 180;
        longitude_rad = longitude_deg * 3.1415926535 / 180;
    }
};

// statistics on given bus route
struct BusRouteStats {
    size_t stops_count{0};
    size_t unique_stops_count{0};
    double bus_route_length{0.0};

    static constexpr double REASONABLE_ERROR = 0.0001;

    bool operator==(const BusRouteStats& rhs) const {
        return stops_count == rhs.stops_count &&
                unique_stops_count == rhs.unique_stops_count &&
                fabs(bus_route_length - rhs.bus_route_length) < REASONABLE_ERROR;
    }
};

class TransportGuide {
  public:
    using BusRoute = std::vector<Stop*>;
    using StopNameToStopPtr = std::unordered_map<std::string_view, Stop*>;
    using BusNameToBusRoute = std::unordered_map<std::string_view, BusRoute*>;
    using BusNameToBusRouteStats = std::unordered_map<std::string_view, BusRouteStats>;

  public:
    TransportGuide() = default;
    void CreateDB(std::istream& in);
    void ProcessRetrievalQueries(std::istream& in);
    void PrintBusRouteInfo(std::string_view bus_route_name) const;
    void PrintStopInfo(std::string_view stop_name) const;

    const std::vector<std::string>& GetBuffer() const;

    const std::deque<Stop>& GetStops() const;
    const StopNameToStopPtr& GetStopNameToStopPtr() const;
    size_t GetUniqueStopsCountForBusRoute(std::string_view bus_route_name) const;

    const std::deque<BusRoute>& GetBusRoutes() const;
    const BusNameToBusRoute& GetBusNameToBusRouteMapping() const;
    const BusRouteStats& GetStatsForBusRoute(std::string_view bus_route_name) const;

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

    for (string_view bus_route_entry : _buffer) {
        const auto& [bus_route_name, stop_names] = ParseBusRouteView(bus_route_entry);
        _bus_routes.emplace_back(stop_names.size());
        BusRoute& current_bus_route = _bus_routes.back();
        _bus_name_to_bus_route[bus_route_name] = &current_bus_route;

        unordered_set<string_view> unique_stops;
        vector<double> distances;
        for (size_t i = 0; i < stop_names.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stop_names[i]];
            unique_stops.insert(stop_names[i]);
            _stop_name_to_bus_routes[stop_names[i]].insert(bus_route_name);
        }

        for (size_t i = 0, j = 1; j < stop_names.size(); ++i, ++j) {
            Stop* lhs{current_bus_route[i]};
            Stop* rhs{current_bus_route[j]};
            distances.push_back(acos(sin(lhs->latitude_rad) * sin(rhs->latitude_rad) +
                                     cos(lhs->latitude_rad) * cos(rhs->latitude_rad) *
                                     cos(abs(lhs->longitude_rad - rhs->longitude_rad)))
                                * 6'371'000);
        }

        double bus_route_length = accumulate(distances.begin(),
                                             distances.end(),
                                             0.0);

        _bus_name_to_bus_route_stats[bus_route_name] = BusRouteStats{
                stop_names.size(),
                unique_stops.size(),
                bus_route_length
        };
    }
}

void TransportGuide::PrintBusRouteInfo(std::string_view bus_route_name) const {
    if (_bus_name_to_bus_route.count(bus_route_name)) {
        const BusRouteStats& bus_stats = GetStatsForBusRoute(bus_route_name);
        cout << "Bus " << bus_route_name << ": "
             << bus_stats.stops_count << " stops on route, "
             << bus_stats.unique_stops_count << " unique stops, "
             << std::setprecision(6) << bus_stats.bus_route_length << " route length"
             << endl;
    } else {
        cout << "Bus " << bus_route_name
             << ": not found"
             << endl;
    }
}

void TransportGuide::PrintStopInfo(std::string_view stop_name) const {
    if (0 == _stop_name_to_stop_ptr.count(stop_name)) {
        cout << "Stop " << stop_name
             << ": not found"
             << endl;
    } else {
        if (0 == _stop_name_to_bus_routes.count(stop_name)) {
            cout << "Stop " << stop_name
                 << ": no buses"
                 << endl;
        } else {
            cout << "Stop " << stop_name
                 << ": buses";
            for (const auto bus_name: _stop_name_to_bus_routes.at(stop_name)) {
                cout << " " << bus_name;
            }
            cout << endl;
        }
    }
}

void TransportGuide::ProcessRetrievalQueries(std::istream& in) {
    size_t retrieve_queries_count;
    in >> retrieve_queries_count >> std::ws;

    string line{};
    for (size_t i = 0; i < retrieve_queries_count; ++i) {
        getline(in, line);
        const auto& [command, arg] = SplitIntoTwoPartsView(line, ' ');

        switch (str_to_retrieve_command[command]) {
            case RetrieveCommand::Bus:
                PrintBusRouteInfo(arg);
                break;
            case RetrieveCommand::Stop: {
                PrintStopInfo(arg);
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
        case CreateCommand::Stop:
            _stops.push_back(ParseStopView(s));
            _stop_name_to_stop_ptr.insert({_stops.back().stop_name, &(_stops.back())});
            break;
    }
}

pair<string_view, vector<string_view>> TransportGuide::ParseBusRouteView(std::string_view bus_route_entry) {
    auto [bus_route_name, delimited_stops] = SplitIntoTwoPartsView(bus_route_entry, ':');
    delimited_stops = trim_view(delimited_stops);

    size_t first_delim_pos = delimited_stops.find_first_of(">-");
    if (first_delim_pos == string_view::npos) { return {}; }

    const char separator = delimited_stops[first_delim_pos];
    vector<string_view> stops = SplitBy(delimited_stops, separator);

    // in case of circular bus route: A - B - C <=> A > B > |C| > B > A
    if (separator == '-') {
        stops.resize(stops.size() * 2 - 1);
        std::copy_backward(next(stops.rbegin()), stops.rend(),
                           stops.end());
    }

    return {bus_route_name, stops};
}

void TransportGuide::CreateDB(std::istream& in) {
    AddStops(in);
    AddBusRoutes();
}
// ============================================================================================================================

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

vector<string_view> SplitBy(string_view s, char sep) {
    vector<string_view> result;

    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(trim_view(s.substr(0, pos)));
        s.remove_prefix(pos != string_view::npos
                        ? pos + 1
                        : s.size());
    }

    return result;
}

pair<string_view, string_view> SplitIntoTwoPartsView(string_view s, char sep) {
    size_t pos = s.find(sep);
    if (pos != string_view::npos) {
        return {trim_view(s.substr(0, pos)),
                trim_view(s.substr(pos + 1))};
    } else {
        return {};
    }
}

// parse string in the following format:
// stop_name: latitude_deg, longitude_deg
// Tolstopaltsevo: 55.611087, 37.20829
Stop ParseStopView(string_view s) {
    auto [stop_name, coordinates] = SplitIntoTwoPartsView(s, ':');
    auto [latitude, longitude] = SplitIntoTwoPartsView(coordinates, ',');

    return {string(stop_name),
            stod(string{latitude}),
            stod(string{longitude})};
}

int main() {
    TransportGuide guide;
    guide.CreateDB(cin);
    guide.ProcessRetrievalQueries(cin);

    return 0;
}
