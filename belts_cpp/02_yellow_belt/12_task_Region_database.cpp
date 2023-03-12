#include <algorithm>
#include <ios>  // std::boolalpha
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

/*
База регионов

Имеется база регионов, представленная вектором структур Region:

struct Region {
  string std_name;
  string parent_std_name;
  map<Lang, string> names;
  int64_t population;
};

Здесь Lang — идентификатор языка:

enum class Lang {
  DE, FR, IT
};

Напишите функцию FindMaxRepetitionCount, принимающую базу регионов и определяющую,
какое максимальное количество повторов(число вхождений одного и того же элемента) она содержит.
Две записи (объекты типа Region) считаются различными, если они отличаются хотя бы одним полем.

int FindMaxRepetitionCount(const vector<Region>& regions);

Если все записи уникальны, считайте максимальное количество повторов равным 1.
Если записи отсутствуют, верните 0. Гарантируется, что типа int достаточно для хранения ответа.
*/

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
    for (const auto& region : regions) {
        ++region_counter[region];
    }

    auto it_largest = std::max_element(region_counter.begin(), region_counter.end(),
                                       [](const auto& lhs, const auto& rhs) {
                                           return lhs.second < rhs.second;
                                       });
    return (*it_largest).second;
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
