#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt beginIt, RandomIt endIt, char prefix) {
    // get the equivalent of std::equal_range prefix specified by single character
    auto lowerBound = std::partition_point(beginIt, endIt, [&](const auto str) { return str.front() < prefix; });
    auto upperBound = std::partition_point(beginIt, endIt, [&](const auto str) { return str.front() <= prefix; });

    return std::pair(lowerBound, upperBound);
}

int main() {
    const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};
    const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;
    const auto p_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
    cout << (p_result.first - begin(sorted_strings)) << " " << (p_result.second - begin(sorted_strings)) << endl;
    const auto z_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
    cout << (z_result.first - begin(sorted_strings)) << " " << (z_result.second - begin(sorted_strings)) << endl;
    return 0;
}