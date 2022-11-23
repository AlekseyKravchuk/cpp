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
    // Дана некоторая строка "s", нужно вывести все позиции пробелов в этой строке

    // позиции пробелов: "3 8 12 16"
    const string s = "aba caba end pos "s;

    // заведем вектор для хранения найденных позиций
    vector<int> positions;

    for(size_t pos = s.find(" "s);
        pos != std::string::npos;
        pos = s.find(" "s, ++pos)) {
            positions.push_back(pos);
        }
    
    PrintRange(begin(positions), end(positions));

    return 0;
}
