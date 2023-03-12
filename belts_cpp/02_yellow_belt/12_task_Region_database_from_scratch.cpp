#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>  // std::tie
#include <vector>

enum class Lang {
    DE,
    FR,
    IT
};

struct Region {
    std::string std_name;
    std::string parent_std_name;
    std::map<Lang, std::string> names;
    int64_t population;
};

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

int main() {
    std::cout << FindMaxRepetitionCount({
                     {"Moscow",
                      "Russia",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                      89},
                     {"Russia",
                      "Eurasia",
                      {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                      89},
                     {"Moscow",
                      "Russia",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                      89},
                     {"Moscow",
                      "Russia",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                      89},
                     {"Russia",
                      "Eurasia",
                      {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                      89},
                 })
              << std::endl;

    std::cout << FindMaxRepetitionCount({
                     {"Moscow",
                      "Russia",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                      89},
                     {"Russia",
                      "Eurasia",
                      {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                      89},
                     {"Moscow",
                      "Russia",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
                      89},
                     {"Moscow",
                      "Toulouse",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                      89},
                     {"Moscow",
                      "Russia",
                      {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                      31},
                 })
              << std::endl;

    return 0;
}