#pragma once

#include <iostream>
#include <string>

#include "busdb.h"


BusDB dbInput(uint lines, std::istream& is);

void dbOutput(uint lines, const BusDB& db, std::istream& is, std::ostream& os);



Route ParceRoute(std::string query);

Stop ParceStop(std::string query);

Stop ParceStopExtended(Stop base, std::string query, size_t extendBeginPos);

std::string stopInfo(std::set<std::string_view> routesForStop);

std::string routeInfo(Route route);

// BusDB::DataType ParceGeneric(std::string query);
