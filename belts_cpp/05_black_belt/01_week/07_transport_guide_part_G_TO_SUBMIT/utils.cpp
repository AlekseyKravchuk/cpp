#include "utils.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

string_view Strip(string_view line) {
    while (!line.empty() && isspace(line.front())) {
        line.remove_prefix(1);
    }
    while (!line.empty() && isspace(line.back())) {
        line.remove_suffix(1);
    }
    return line;
}

std::string GetQuotedString(const std::string& str) {
    stringstream os_quoted;
    os_quoted << quoted(str);
    string line = os_quoted.str();
    int line_start = 0, line_len = line.length();
    if (line.front() == '"') {
        ++line_start;
        --line_len;
    }
    if (line.back() == '"') {
        --line_len;
    }
    return line.substr(line_start, line_len);
}

