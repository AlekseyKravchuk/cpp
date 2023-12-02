#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>  // std::tie
#include <vector>

bool operator<(const Region& lhs, const Region& rhs) {
    return std::tie(lhs.std_name, lhs.parent_std_name, lhs.names, lhs.population) <
           std::tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
}

int FindMaxRepetitionCount(const std::vector<Region>& regions) {
    std::map<Region, int> region_counter;
    int count = 0;
    for (const auto& region : regions) {
        count = std::max(count, ++region_counter[region]);
    }
    return count;
}
