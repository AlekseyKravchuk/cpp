#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <utility>
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
    vector<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s, "Go"s, "Ruby"s, "Haskel"s, "Cobol"s};

    // хотим найти позицию найденного элемента в векторе
    auto foundElementIt = std::find_if(begin(langs),
                                       end(langs),
                                       [](const string& lang) {
                                           return lang[0] == 'C';
                                       });
    // оказывается, что для нахождения позиции найденного элемента, достаточно из возвращенного методом std::find_if итератора
    // вычесть итератор начала контейнера
    // итераторы вектора можно вычитать друг из друга
    size_t foundElementPosition = foundElementIt - begin(langs);

    std::cout << "Found element \""s << *foundElementIt << "\" at position <"s << foundElementPosition << "> in vector: "s;
    PrintRange(langs.begin(), langs.end());
    std::cout << std::endl;

    // история с вычитанием итераторов не пройдет в случае std::set
    // для итераторов множества оператор "минус" не определен
    // создадим множество целых чисел:
    std::set<int> s = {1, 3, 6, 8, 9, 11, 15};
    auto it = s.find(6);

    // пусть теперь стоит задача вывести все элементы бОльшие, чем "6"
    PrintRange(std::next(it), s.end());

    return 0;
}