#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <vector>

using namespace std;

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

int main() {
    // Пусть дана некоторая строка. Требуется найти в ней все позиции пробелов
    const string s = "one two three four"s;
    // Desired output: 3, 7, 13

    // вектор для хранения позиций пробелов
    std::vector<int> positions;

    for (auto it = std::find(begin(s), end(s), ' ');
         it != s.end();
         it = std::find(std::next(it), end(s), ' ')) {
        positions.push_back(it - begin(s));
    }

    PrintRange(positions.begin(), positions.end());


    return 0;
}