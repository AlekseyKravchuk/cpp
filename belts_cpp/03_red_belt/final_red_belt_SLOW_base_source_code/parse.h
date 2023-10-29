#pragma once

#include <sstream>
#include <string_view>
#include <vector>
#include <string_view>

#include "iterator_range.h"

template <typename Container>
std::string Join(char c, const Container &cont)
{
    std::ostringstream os;

    for (const auto &item : Head(cont, static_cast<int>(cont.size()) - 1))
    {
        os << item << c;
    }

    os << *rbegin(cont);
    return os.str();
}

std::string_view Strip(std::string_view s);
std::vector<std::string_view> SplitBy(std::string_view s, char sep);
