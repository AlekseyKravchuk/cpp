#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <utility>  // std::pair

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

pair<string_view, string_view> SplitIntoTwoParts(string_view s, char sep) {
    size_t pos = s.find(sep);
    if (pos != string_view::npos) {
        return {trim_view(s.substr(0, pos)),
                trim_view(s.substr(pos + 1))};
    } else {
        return {};
    }
}

Stop ParseStopView(string_view s) {
    auto [stop_name, coordinates] = SplitIntoTwoParts(s, ':');
    auto [latitude, longitude] = SplitIntoTwoParts(coordinates, ',');

    return {string(stop_name),
            stod(string{latitude}),
            stod(string{longitude})};
}

StopsInfo ParseStopQueries(istream& in, vector<string>& buffer) {
    size_t add_queries_count;
    in >> add_queries_count >> ws;
    deque<Stop> stops;
    StopNameToStopPtr stop_name_to_stop_ptr;

    // обрабатываем ту часть входного потока, которая касается остановок
    string line;
    for(size_t i = 0; i < add_queries_count; ++i) {
        getline(in, line);
        string_view s{line};
        string_view command_part_holder = s.substr(0, MAX_COMMAND_LENGTH);

        if (string command = "Stop"; command_part_holder.find(command) != string_view::npos) {
            s.remove_prefix(command.size() + 1); // remove command name with space that follows it
            stops.push_back(ParseStopView(s));
            stop_name_to_stop_ptr[stops.back().stop_name] = &stops.back();
//            stop_name_to_stop_ptr[stops.back().stop_name] = make_shared<Stop>(stops.back());

//            stop_name_to_stop_ptr.insert({string_view{stops.back().stop_name},
//                                          make_shared<Stop>(stops.back())});

//            Stop& stop = stops.back();
//            stop_name_to_stop_ptr[stop.stop_name] = make_shared<Stop>(stop);
        } else {
            // запросы на создание маршрутов сохраняем во временном буфере
            buffer.push_back(std::move(line));
        }
    }

    return {std::move(stops), std::move(stop_name_to_stop_ptr)};
}