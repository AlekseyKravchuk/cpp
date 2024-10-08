#include <algorithm>
#include <deque>
#include <map>
#include <string>
#include <vector>

#include "test_runner.h"

using namespace std;

// Объявляем "Group<String>" для произвольного типа String синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа позволяет вместо громоздкого "typename String::value_type" использовать "Char<String>"
template <typename String>
using Char = typename String::value_type;

template <typename String>
using Key = String;

template <typename String>
Key<String> ComputeStringKey(const String& string) {
    String chars = string;
    sort(begin(chars), end(chars));
    chars.erase(unique(begin(chars), end(chars)), end(chars));
    return chars;
}

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    map<Key<String>, Group<String>> groups_map;
    for (String& string : strings) {
        groups_map[ComputeStringKey(string)].push_back(move(string));
    }
    vector<Group<String>> groups;
    for (auto& [key, group] : groups_map) {
        groups.push_back(move(group));
    }
    return groups;
}

void TestGroupingABC() {
    std::vector<std::string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2u);
    std::sort(std::begin(groups), std::end(groups));  // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], std::vector<std::string>({"caab", "abc", "bacc"}));
    ASSERT_EQUAL(groups[1], std::vector<std::string>({"cccc", "c"}));
}

void TestGroupingReal() {
    std::vector<std::string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4u);
    std::sort(std::begin(groups), std::end(groups));  // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], std::vector<std::string>({"laptop", "paloalto"}));
    ASSERT_EQUAL(groups[1], std::vector<std::string>({"law", "wall", "awl"}));
    ASSERT_EQUAL(groups[2], std::vector<std::string>({"port"}));
    ASSERT_EQUAL(groups[3], std::vector<std::string>({"top", "pot"}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestGroupingABC);
    RUN_TEST(tr, TestGroupingReal);

    return 0;
}
