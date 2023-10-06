#pragma once

#include <map>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "iterator_range.h"

template <typename Container>
std::string Join(char c, const Container& cont) {
    std::ostringstream os;

    for (const auto& item : Head(cont, cont.size() - 1)) {
        os << item << c;
    }

    os << *rbegin(cont);
    return os.str();
}

void trim(std::string_view& s_view);
std::unordered_map<std::string_view, size_t> GetWordsCounterView(std::string_view s_view);
std::vector<std::string_view> SplitIntoWordsView(std::string_view s_view);

std::string_view Strip(std::string_view s);
std::vector<std::string_view> SplitBy(std::string_view s, char sep);
