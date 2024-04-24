#pragma once

#include "transport_guide.h"

#include <string>
#include <string_view>
#include <deque>
#include <unordered_map>
#include <memory>

struct StopsInfo {
    std::deque<Stop> stops;
    std::unordered_map<std::string_view, std::shared_ptr<Stop>> stopname_to_stopptr;
};

// trim from beginning of string
std::string& trim_leading(std::string& s);

// trim from end of string
std::string& trim_trailing(std::string& s);

// trim from both ends of string
std::string& trim(std::string& s);

std::string_view trim_view(std::string_view s);

std::vector<std::string_view> SplitBy(std::string_view s, char sep);


// Stop Tolstopaltsevo: 55.611087, 37.20829
// После предобработки:
// Tolstopaltsevo: 55.611087, 37.20829
//StopsInfo ParseStop(string s) {
//    std::deque<Stop> stops;
//    std::unordered_map<string_view, shared_ptr<Stop>> stopname_to_stopptr;
//}

// ====
Stop ParseStopView(std::string_view s);

