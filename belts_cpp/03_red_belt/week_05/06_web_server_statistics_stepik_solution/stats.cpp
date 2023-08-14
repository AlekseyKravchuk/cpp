#include "stats.h"

void Stats::AddMethod(std::string_view method) {
    _methods.Add(method);
}

void Stats::AddUri(std::string_view uri) {
    _uris.Add(uri);
}

const std::map<std::string_view, int>& Stats::GetMethodStats() const {
    return _methods.GetValues();
}

const std::map<std::string_view, int>& Stats::GetUriStats() const {
    return _uris.GetValues();
}

void LeftStrip(std::string_view& sv) {
    while (!sv.empty() && isspace(sv[0])) {
        sv.remove_prefix(1);
    }
}

std::string_view ReadToken(std::string_view& sv) {
    LeftStrip(sv);

    auto pos = sv.find(' ');
    auto result = sv.substr(0, pos);
    sv.remove_prefix(pos != sv.npos ? pos : sv.size());
    return result;
}

HttpRequest ParseRequest(std::string_view line) {
    auto method = ReadToken(line);
    auto uri = ReadToken(line);
    return {method, uri, ReadToken(line)};
}

void StatPiece::Add(std::string_view value) {
    if (auto it = _counts.find(value); it != _counts.end()) {
        ++it->second;
    } else {
        ++_counts[_default_key];
    }
}