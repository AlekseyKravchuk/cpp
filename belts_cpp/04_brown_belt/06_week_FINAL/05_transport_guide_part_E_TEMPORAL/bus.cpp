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

BusRoute::BusRoute(RouteNumber bus_no, vector<string> stops)
  : _number(move(bus_no))
  , _stops(move(stops))
  , _stop_names(begin(_stops), end(_stops))
{
}

BusRoute BusRoute::CreateRawBusRoute(RouteNumber bus_no,
                                     const std::vector<std::string>& stops) {
  vector<string> cyclic_route{begin(stops), end(stops)};
  cyclic_route.insert(end(cyclic_route), next(rbegin(stops)), rend(stops));
  return {move(bus_no), move(cyclic_route)};
}

BusRoute BusRoute::CreateCyclicBusRoute(RouteNumber bus_no, const std::vector<std::string>& stops) {
  return {move(bus_no), stops};
}
