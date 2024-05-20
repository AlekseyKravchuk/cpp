#pragma once

#include <algorithm>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "transport_guide.h"

std::string_view ltrim_view(std::string_view s);
std::string_view rtrim_view(std::string_view s);
std::string_view trim_view(std::string_view s);

std::pair<std::string_view, std::string_view> SplitIntoTwoPartsView(std::string_view s, char sep);
std::vector<std::string_view> SplitBy(std::string_view s, char sep);

Stop ParseStopView(std::string_view s);
