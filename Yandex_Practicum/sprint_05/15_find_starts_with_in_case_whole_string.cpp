#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt beginIt, RandomIt endIt, string prefix) {
    // get the equivalent of std::equal_range for arbitraty prefix length
    auto lowerBound = std::partition_point(beginIt, endIt, [&](const auto str) { return str.substr(0, prefix.size()) < prefix; });
    auto upperBound = std::partition_point(beginIt, endIt, [&](const auto str) { return str.substr(0, prefix.size()) <= prefix; });

    return std::pair(lowerBound, upperBound);
}

int main() {
    const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};
    const auto mo_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;
    const auto mt_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
    cout << (mt_result.first - begin(sorted_strings)) << " " << (mt_result.second - begin(sorted_strings)) << endl;
    const auto na_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
    cout << (na_result.first - begin(sorted_strings)) << " " << (na_result.second - begin(sorted_strings)) << endl;
    return 0;
}