#include <algorithm>  // std::equal_range
#include <iostream>
#include <string>
#include <utility>  // std::pair
#include <vector>

using namespace std;

/*
================== Группировка строк по префиксу ==================
Часть 2. Группировка по префиксу

Напишите более универсальный вариант функции FindStartsWith, принимающий в качестве префикса произвольную строку,
состоящую из строчных латинских букв.

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    const string& prefix);
    
*/

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin,
                                        RandomIt range_end,
                                        const string& prefix) {
    auto lb = std::lower_bound(range_begin, range_end, prefix);
    auto ub = std::upper_bound(range_begin,
                               range_end,
                               prefix,
                               [](const std::string& pref, const std::string& rhs) {
                                   if (rhs.compare(0, pref.size(), pref) == 0) {
                                       return pref < rhs.substr(0, pref.size());
                                   } else {
                                       return pref < rhs;
                                   }
                               });
    return {lb, ub};
}

int main() {
    const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};
    // const vector<string> sorted_strings = {"anadyr", "borisoglebsk", "dzehzhinsk", "moscow", "motovilikha", "murmansk"};

    const auto mo_result =
        FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto mt_result =
        FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
    cout << (mt_result.first - begin(sorted_strings)) << " " << (mt_result.second - begin(sorted_strings)) << endl;

    const auto na_result =
        FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
    cout << (na_result.first - begin(sorted_strings)) << " " << (na_result.second - begin(sorted_strings)) << endl;

    return 0;
}