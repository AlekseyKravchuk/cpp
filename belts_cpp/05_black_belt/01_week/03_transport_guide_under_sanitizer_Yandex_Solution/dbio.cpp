
#include "dbio.h"

#include <stdexcept>
#include <utility>
#include <cstdlib>
#include <unordered_set>
#include <string_view>
#include <sstream>

using namespace std;


// BusDB::DataWrapper ParceGeneric(string query) {
//   const string type = query.substr(0,query.find(" "));
//   if (type == "Stop") {
    
//   } else if (type == "Bus") {
    
//   }

//   return BusDB::DataWrapper();
// }

enum class QueryType {
  STOP,
  ROUTE
};

pair<QueryType,string> getTypeAndKey(const string_view query) {
  size_t typeEnd = query.find(" ");
  const string_view type = query.substr(0,typeEnd);
  if (type == "Stop") {
    return {QueryType::STOP, string(query.substr(typeEnd+1))};
  } else if (type == "Bus") {
    return {QueryType::ROUTE, string(query.substr(typeEnd+1))};
  } else {
    throw runtime_error("Unknown data type: " + string(type));
  }
}

Stop ParceStop(string query) {
  size_t nameBegin, nameEnd, latitudeBegin, latitudeEnd, longitudeBegin, longitudeEnd;
  nameBegin = query.find(" ") + 1;
  nameEnd = query.find(":", nameBegin);
  latitudeBegin = nameEnd + 2;
  latitudeEnd = query.find(",", latitudeBegin);
  longitudeBegin = latitudeEnd + 2;
  longitudeEnd = query.find(",", longitudeBegin);

  Stop result;
  result.name = query.substr(nameBegin, nameEnd - nameBegin);
  result.location.latitude = strtod(&query[latitudeBegin], nullptr);
  //char* testForExtended;
  result.location.longitude = strtod(&query[longitudeBegin], nullptr); //&testForExtended);
  //return (*testForExtended!= ',') ? result
  return (longitudeEnd >= query.size())? result
    : ParceStopExtended(move(result), move(query),
			longitudeEnd);
			//(testForExtended - query.data()) / sizeof(char)); // UNPORTABLE
}

Stop ParceStopExtended(Stop base, string query, size_t extendBeginPos) {
  size_t distanceBegin, distanceEnd, stopNameBegin, stopNameEnd;
  stopNameEnd = extendBeginPos;
  while (stopNameEnd <= query.size()) {
    distanceBegin = stopNameEnd + 2;
    distanceEnd = query.find("m", distanceBegin);
    stopNameBegin = distanceEnd + 5;
    stopNameEnd = query.find(",", stopNameBegin);

    base.measuredDistances.emplace(query.substr(stopNameBegin,
					       stopNameEnd - stopNameBegin),
                                   strtoul(&query[distanceBegin], nullptr, 0)
				  );
  }
  return base;
}

Route ParceRoute(string query) {
  size_t numberBegin, numberEnd, stopNameBegin, stopNameEnd;
  numberBegin = query.find(" ") + 1;
  numberEnd = query.find(":", numberBegin);
  stopNameBegin = numberEnd + 2;
  size_t firstDelimiterPos;
  for (firstDelimiterPos = stopNameBegin; firstDelimiterPos <= query.size()
	 && query[firstDelimiterPos] != '>' && query[firstDelimiterPos] != '-';
         firstDelimiterPos++);
  char delimiter = query[firstDelimiterPos];
  stopNameEnd = firstDelimiterPos - 1;
  
  Route result;
  result.number = query.substr(numberBegin, numberEnd - numberBegin);
  {
    string_view query_wrapped(query);
    unordered_set<string_view> unique_counter;
    while (true) {
      result.stops.push_back(query.substr(stopNameBegin,
					  stopNameEnd - stopNameBegin));
      unique_counter.insert(query_wrapped.substr(stopNameBegin,
						 stopNameEnd - stopNameBegin));
      
      stopNameBegin = stopNameEnd + 3;
      if (stopNameBegin <= numberEnd) break;
      stopNameEnd = query.find(delimiter,stopNameBegin) - 1;
    }
    result.uniqueStops = unique_counter.size();
  }
  if (delimiter == '-') {
    result.stops.reserve(result.stops.size() * 2 - 1);
    for (size_t i = result.stops.size(); i >= 2; i--) {
      result.stops.push_back(result.stops[i-2]);
    }
  }
  
  return result;
}

void dbParceAndAdd(string query, BusDB& db) {
  switch (getTypeAndKey(query).first) {
  case QueryType::STOP :
    db.AddStopInfo(ParceStop(move(query)));
    break;
  case QueryType::ROUTE :
    db.AddRoute(ParceRoute(move(query)));
    break;
  default:
    throw runtime_error("Unsuported input query: " + query);
  }
}

BusDB dbInput(uint lines, istream& is) {
  BusDB db;
  for (uint i=0; i < lines; i++) {
    string query;
    getline(is, query);
    dbParceAndAdd(move(query), db);
  }
    db.PrepareRoutesRouter();
  return db;
}

string stopInfo(set<string_view> routesForStop) {
  if (routesForStop.empty())
    return "no buses";
  ostringstream os;
  os << "buses";
  for (string_view route : routesForStop)
    os << " " << route;
  return os.str();
}

string routeInfo(const Route route) {
  ostringstream os;
  //os.precision(7);
  os << "Bus " << route.number << ": "
     << route.stopsOnRoute() << " stops on route, "
     << route.uniqueStops << " unique stops, "
     << route.measuredDistance.value() << " route length, "
     << route.measuredDistance.value() / route.geographicalDistance.value()
     << " curvature";
  return os.str();
}

string getRoutesForStopFromDB(string stopName, const BusDB& db) {
  optional<set<string_view>> routes = db.RoutesForStop(stopName);
  return "Stop " + stopName + ": " +
    ((routes) ? stopInfo(routes.value()) : "not found");
}

string getRouteFromDB(string number, const BusDB& db) {
  optional<Route> route = db.FindRoute(number);
  if (route)
    return routeInfo(std::move(route.value()));
  else return "Bus " + number + ": not found";
}

string processQuery(string query, const BusDB& db) {
  pair<QueryType, string> parcedQuery = getTypeAndKey(query);
  switch (parcedQuery.first) {
  case QueryType::STOP :
    return getRoutesForStopFromDB(parcedQuery.second,db);
  case QueryType::ROUTE :
    return getRouteFromDB(parcedQuery.second, db);
  default:
    throw runtime_error("Unsuported output query: " + query);
  }
}

void dbOutput(uint lines, const BusDB& db, istream& is, ostream& os) {
  for (uint i=0; i < lines; i++) {
    string query;
    getline(is, query);
    os << processQuery(move(query), db) << "\n";
  }
}
