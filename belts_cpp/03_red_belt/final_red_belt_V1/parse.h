#pragma once

#include <sstream>
#include <string_view>
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
std::map<std::string_view, size_t> GetWordsViewCounter(std::string_view s_view);
// std::vector<std::string_view> SplitIntoWords(std::string_view s_view);
// std::vector<std::string> SplitIntoWords(const std::string& line);

std::string_view Strip(std::string_view s);
std::vector<std::string_view> SplitBy(std::string_view s, char sep);
