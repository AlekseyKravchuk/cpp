#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>

using namespace std;

int main() {
    map<string_view, string_view> forwardTranlation;
    map<string_view, string_view> reverseTranlation;

    // set<string> myset = {"okno"s, "window"s, "table"s, "stol"s};
    set<string> myset;

    // добавляем "okno" -> "window"
    myset.insert("okno"s);
    myset.insert("window"s);
    forwardTranlation["okno"s] = "window"s;

    // добавляем "okno" -> "slot"
    myset.insert("okno"s);
    myset.insert("slot"s);

    forwardTranlation["okno"s] = "slot"s;

    auto insertRes = myset.insert("window"s);
    std::cout << insertRes.second << std::endl;
    return 0;
}
