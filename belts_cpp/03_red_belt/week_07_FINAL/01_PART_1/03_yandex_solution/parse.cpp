#include "parse.h"

string_view Strip(string_view s) {
  while (!s.empty() && isspace(s.front())) {
    s.remove_prefix(1);
  }

  while (!s.empty() && isspace(s.back())) {
    s.remove_suffix(1);
  }

  return s;
}

vector<string_view> SplitBy(string_view s, char sep) {
  vector<string_view> result;

  while (!s.empty()) {
    size_t pos = s.find(sep);
    result.push_back(s.substr(0, pos));
    s.remove_prefix(pos != std::string_view::npos ? pos + 1 : s.size());
  }

  return result;
}

std::vector<std::pair<std::string_view, size_t>> SplitIntoWords(std::string_view str) {
    std::unordered_map<std::string_view, size_t> result;

    while (!str.empty()) {
        auto space = str.find(' ');
        if (auto substr = str.substr(0, space); !substr.empty()) {
            result[substr] += 1;
        }
        str.remove_prefix(space == std::string_view::npos ? str.size() : space + 1);
    }

    return {result.begin(), result.end()};
}