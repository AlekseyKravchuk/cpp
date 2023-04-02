#pragma once

#include <string>
#include <vector>
#include <map>
#include <istream>  // std::ws, std::istream

using namespace std::literals;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    std::string bus;
    std::string stop;
    std::vector<std::string> stops;
};

extern std::map<std::string, QueryType> str2qtype;

std::istream& operator>>(std::istream& is, QueryType& qtype);

std::istream& operator>>(std::istream& is, Query& q);