#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <map>

#include "test_runner.h"

// Объявляем Group<String> для произвольного типа String синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = std::vector<String>;

// Ещё один шаблонный синоним типа позволяет вместо громоздкого "typename String::value_type" использовать "Char<String>"
template <typename String>
using Char = typename String::value_type;

template <typename String>
std::vector<Group<String>> GroupHeavyStrings(std::vector<String> strings) {
    // Напишите реализацию функции, использовав не более 1 копирования каждого символа
    std::map<std::set<typename String::value_type>, std::vector<String>> groups_dict;
    for (auto& str : strings) {
        std::set<typename String::value_type> group(std::make_move_iterator(std::begin(str)), std::make_move_iterator(std::end(str)));
        groups_dict[group].push_back(std::move(str));
    }

    std::vector<Group<String>> v_groups;
    v_groups.reserve(groups_dict.size());

    for (auto& [group, strings] : groups_dict) {
        v_groups.push_back(std::move(strings));
    }

    return v_groups;
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
