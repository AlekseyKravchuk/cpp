#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

struct Lang {
    string languageName;
    int languageAge;
};

// перегружаем "stream insertion operator <<" для вывода структуры Lang
// "stream insertion operator <<" можно перегрузить ТОЛЬКО глобальной функцией
// так как операнд слева std::cout (std::ostream&)
std::ostream& operator<<(std::ostream& os, const Lang& lang) {
    os << "{"s << lang.languageName << ", " << lang.languageAge << "}"s;
    return os;
}

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    // инициализируем вектор структур "Lang"
    vector<Lang> langs = {
        {"Python"s, 26},
        {"C++"s, 34},
        {"C"s, 45},
        {"Java"s, 22},
        {"C#"s, 17}};
    // Нужно найти в данном векторе структур, объект, имя которого начинается с буквы 'C'

    // std::find_if возвращает неконстантный итератор
    auto result = std::find_if(langs.begin(),
                               langs.end(),
                               [](const Lang& lang) {
                                   return lang.languageName[0] == 'K';
                               });
    if (result == langs.end()) {
        std::cout << "std::find_if didn't find element" << std::endl;
    } else {
        Lang& foundElementRef = *result;
        std::cout << "Found element: "s << foundElementRef << std::endl;

        // оператор "->" предоставляет удобный синтаксис для обращения к полям структуры
        // эквивалентно следующему выражению: (*result).languageName = "D++"s;
        result->languageName = "D++"s;
        PrintRange(langs.begin(), langs.end());
    }

    return 0;
}

// ============================= std :: find_if =============================
// Returns an iterator to the first element in the range [first, last) for which pred(Unary Function) returns true.
// If no such element is found, the function returns last.

// Function Template:
// InputIterator find_if (InputIterator first, InputIterator last, UnaryPredicate pred);

// first, last :range which contains all the elements between first
// and last, including the element pointed by first but
// not the element pointed by last.

// pred : Unary function that accepts an element in the range
// as argument and returns a value in boolean.

// Return value :
// Returns an iterator to the first element in the range
// [first, last) for which pred(function) returns true. If
// no such element is found, the function returns last.
// ============================================================================