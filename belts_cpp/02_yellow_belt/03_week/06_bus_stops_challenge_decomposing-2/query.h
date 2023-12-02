#pragma once

#include <istream>  // std::ws, std::istream
#include <map>
#include <ostream>
#include <string>
#include <vector>

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

extern std::map<QueryType, std::string> qtype2str;

std::istream& operator>>(std::istream& is, QueryType& qtype);

std::istream& operator>>(std::istream& is, Query& q);

std::ostream& operator<<(std::ostream& os, const Query& q);