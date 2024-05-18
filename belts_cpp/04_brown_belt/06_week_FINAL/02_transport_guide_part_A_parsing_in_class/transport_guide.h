#pragma once

#include <algorithm>
#include <deque>
#include <string>
#include <sstream>  // std::istringstream
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <istream>  // std::ws

struct Stop {
    std::string stop_name;  // название остановки
    double latitude;        // широта
    double longitude;       // долгота
};

class TransportGuide {
  public:
    using BusRoute = std::vector<Stop*>;
    using StopNameToStopPtr = std::unordered_map<std::string_view, Stop*>;
    using BusNameToBusRoute = std::unordered_map<std::string_view, BusRoute*>;

  public:
    TransportGuide() = default;
    void CreateDB(std::istream& in);

    const std::vector<std::string>& GetBuffer() const;

    const std::deque<Stop>& GetStops() const;
    const StopNameToStopPtr& GetStopNameToStopPtr() const;
    const std::vector<size_t>& GetUniqueStopsForBusRoutes() const;

    const std::deque<BusRoute>& GetBusRoutes() const;
    const BusNameToBusRoute& GetBusNameToBusRoutePtr() const;

  private:
    void ParseInputEntry(std::string_view);
    void AddStops(std::istream& in);
    void AddBusRoutes();

  private:
    enum class CreateDBCommand : char {
        Bus,
        Stop
    };

    std::unordered_map<std::string_view, CreateDBCommand> str_to_command = {
            {"Bus",  CreateDBCommand::Bus},
            {"Stop", CreateDBCommand::Stop}
    };

    // Мы не можем одновременненно сохранять информацию об остановках и о маршрутах, состоящих из этих остановок.
    // Сначала нужно сформировать структуры данных из остановок, и только потом приступать к обработке маршрутов.
    std::vector<std::string> _buffer;             // buffer for storing whole records about bus routes from input

    std::deque<Stop> _stops;                      // std::deque doesn't invalidate refs after insertion
    StopNameToStopPtr _stop_name_to_stop_ptr;     // hash table: stop name(string_view) => address of "Stop" instance

    std::deque<BusRoute> _bus_routes;             // each element of deque is vector<Stop*>
    std::vector<size_t> _unique_stops_for_bus_routes;
    std::vector<double> _bus_routes_lengths;
    BusNameToBusRoute _bus_name_to_busroute_ptr;  // hash table: bus name(string_view) => address of "BusRoute" instance
};

