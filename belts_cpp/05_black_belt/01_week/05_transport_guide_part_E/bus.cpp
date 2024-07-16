#include "bus.h"
#include "stop_manager.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

BusRoute::BusRoute(BusRouteName bus_no, vector<string> stops)
        : _bus_route_name(std::move(bus_no)),
        _stops(std::move(stops)),
        _stop_names(begin(_stops), end(_stops)) {}

BusRoute BusRoute::CreateOrdinaryBusRoute(BusRouteName bus_no,
                                          const std::vector<std::string>& stops) {
  vector<string> roundtrip_route{begin(stops), end(stops)};
  roundtrip_route.insert(roundtrip_route.end(), next(rbegin(stops)), rend(stops));
  return {std::move(bus_no), std::move(roundtrip_route)};
}

BusRoute BusRoute::CreateRoundtripBusRoute(BusRouteName bus_no,
                                           const std::vector<std::string>& stops) {
  return {std::move(bus_no), stops};
}
