#include "parse.h"

using namespace std;

// TODO: implement parsing functions
// help: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring

// trim from beginning of string
string& trim_leading(string& s) {
    s.erase(0, s.find_first_not_of(' '));
    return s;
}

// trim from end of string
string& trim_trailing(string& s) {
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
}

// trim from both ends of string
string& trim(string& s) {
    trim_leading(s);
    trim_trailing(s);
    return s;
}

string_view trim_view(string_view s) {
    s.remove_prefix(std::min(s.find_first_not_of(' '), s.size()));

    size_t last_not_space_pos = s.find_last_not_of(' ');
    s.remove_suffix(std::min(s.size() - last_not_space_pos - 1, s.size()));
    return s;
}

vector<string_view> SplitBy(string_view s, char sep) {
    vector<string_view> result;
    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(trim_view(s.substr(0, pos)));
        s.remove_prefix(pos != s.npos
                        ? pos + 1
                        : s.size());
    }
    return result;
}


// Stop Tolstopaltsevo: 55.611087, 37.20829
// После предобработки:
// Tolstopaltsevo: 55.611087, 37.20829
//StopsInfo ParseStop(string s) {
//    deque<Stop> stops;
//    unordered_map<string_view, shared_ptr<Stop>> stopname_to_stopptr;
//}