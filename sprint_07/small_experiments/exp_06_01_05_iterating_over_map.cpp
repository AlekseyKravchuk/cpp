#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

using namespace std;

// нужно переопределить оператор вывода (stream insertion operator << ) для пары std::pair
template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::pair<KeyType, ValueType>& mapPair) {
    os << "<"s << mapPair.first << ", " << mapPair.second << ">"s;
    return os;
}

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

void TestIteratingOverMap() {
    // инициализируем вектор структур "Lang"
    map<string, int> langs = {
        {"Python"s, 26},
        {"C++"s, 34},
        {"Java"s, 45},
        {"C"s, 22},
        {"Ada", 51},
        {"C#"s, 17},
        {"Go"s, 6}};

    auto border = langs.find("C++"s);

    PrintRange(langs.begin(), border);
    PrintRange(border,langs.end());
}

int main() {
    TestIteratingOverMap();

    return 0;
}
