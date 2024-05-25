#include <algorithm>
#include <optional>
#include <string>
#include <utility>  // std::pair
#include <exception>

#include "parse.h"

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

vector<string_view> SplitBy(string_view s, const string& sep) {
    vector<string_view> result;

    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(trim_view(s.substr(0, pos)));
        s.remove_prefix(pos != string_view::npos
                        ? pos + sep.size()
                        : s.size());
    }

    return result;
}

pair<string_view, string_view> SplitIntoTwoPartsView(string_view s, char sep) {
    size_t pos = s.find(sep);
    if (pos != string_view::npos) {
        return {trim_view(s.substr(0, pos)),
                trim_view(s.substr(pos + 1))};
    } else {
        return {};
    }
}

// parse string in the following of 2 formats:
// 1) "stop_name: latitude_deg, longitude_deg"
//     Tolstopaltsevo: 55.611087, 37.20829
// 2) "stop_name: latitude_deg, longitude_deg DISTANCEm to other_stop_name"
pair<Stop, optional<pair<StopName, DistanceToStopByRoad>>> ParseAddStopQuery(string_view s) {
    auto [stop_name, rest_part] = SplitIntoTwoPartsView(s, ':');
    vector<string_view> stop_info = SplitBy(rest_part, ",");

    auto latitude = stop_info[0];
    auto longitude = stop_info[1];

    if (stop_info.size() == 2u) {
        return {Stop{string(stop_name),
                     stod(string{latitude}),
                     stod(string{longitude})},
                nullopt};
    } else if (stop_info.size() == 3u) {
        vector<string_view> v = SplitBy(stop_info[2], string("to"));
        v[0].remove_suffix(1);  // remove letter 'm' at the end of distance
        StopName stop_to = v[1];
        size_t distance = static_cast<size_t>(std::stoi(string(v[0])));
        return {Stop{string(stop_name),
                     stod(string{latitude}),
                     stod(string{longitude})},
                pair{stop_to, distance}};
    } else {
        throw std::logic_error("Wrong format for adding stop: " + string(stop_name));
    }

}

