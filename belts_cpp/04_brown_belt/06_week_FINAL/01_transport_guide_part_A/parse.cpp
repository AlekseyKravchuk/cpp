#include "parse.h"

#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

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

// parsing string_view in the following format: "StopName: latitude, longitude"
// For example:
// Tolstopaltsevo: 55.611087, 37.20829
Stop ParseStopView(string_view s) {
    Stop stop;
    vector<string_view> first;
    vector<string_view> second;
    first.reserve(2);
    second.reserve(2);

    first = SplitBy(s, ':');          // first[0] = "Tolstopaltsevo", first[1] = "latitude, longitude"
    stop.stop_name = first[0];

    second = SplitBy(first[1], ',');  // second[0] = "55.611087", second[1] = "37.20829"
    stop.latitude = stod(string{second[0]});
    stop.longitude = stod(string{second[1]});

    return stop;
}


// Stop Tolstopaltsevo: 55.611087, 37.20829
// После предобработки:
// Tolstopaltsevo: 55.611087, 37.20829
//StopsInfo ParseStop(string s) {
//    deque<Stop> stops;
//    unordered_map<string_view, shared_ptr<Stop>> stopname_to_stopptr;
//}