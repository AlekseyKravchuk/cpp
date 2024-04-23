#pragma once

#include <deque>
#include <string>
#include <string_view>
#include <memory>  // std::shared_ptr
#include <vector>
#include <unordered_map>

struct Stop {
    std::string stop_name;
    double latitude;   // широта
    double longitude;  // долгота
};

using BusRoute = std::vector<std::shared_ptr<Stop>>;

class TransportGuide {
  public:

  private:
    std::deque<Stop> _stops;
    std::vector<BusRoute> _bus_routes;
    std::unordered_map<std::string_view, std::shared_ptr<Stop>> _sname_to_sptr;  // hash table: stop name => address of struct Stop instance
};

