#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

struct Lang {
    string languageName;
    int languageAge;
};

// перегружаем insertion operator << для вывода структуры Lang
// insertion operator << можно перегрузить ТОЛЬКО глобальной функцией
// так как операнд слева std::cout (std::ostream&)
std::ostream& operator<<(std::ostream& os, const Lang& lang) {
    os << "{"s << lang.languageName << ", " << lang.languageAge << "}"s;
    return os;
}

// // заменяем громоздкое название типа итератора на более лаконичное с помощью директивы "using"
// using LangStructIt = vector<Lang>::iterator;
// void PrintRange(LangStructIt range_begin,
//                 LangStructIt range_end) {
//     for (auto it = range_begin; it != range_end; ++it) {
//         if (std::next(it) != range_end) {
//             std::cout << *it << " "s;
//             // std::cout << it->languageName << ", "s;
//         } else {
//             std::cout << *it << std::endl;
//             // std::cout << it->languageName << std::endl;
//         }
//     }
// }

// // заменяем громоздкое название типа итератора на более лаконичное с помощью директивы "using"
// using LangIt = vector<string>::iterator;

// void PrintRange(LangIt range_begin,
//                 LangIt range_end) {
//     for (auto it = range_begin; it != range_end; ++it) {
//         if (std::next(it) != range_end) {
//             std::cout << *it << ", "s;
//         } else {
//             std::cout << *it << std::endl;
//         }
//     }
// }

// ШАБЛОННАЯ ВЕРСИЯ PrintRange()
template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            // std::cout << *it << " "s;
            std::cout << *it << ", "s;
        } else {
            // std::cout << *it << std::endl;
            std::cout << *it << std::endl;
        }
    }
}

void TestIteratingOverStruct() {
    // инициализируем вектор структур "Lang"
    vector<Lang> langs = {
        {"Python"s, 26},
        {"C++"s, 34},
        {"Java"s, 45},
        {"C"s, 22},
        {"C#"s, 17}};

    PrintRange(langs.begin(), langs.end());
}

void TestIteratingOverStrings() {
    vector<string> langs = {"Python"s, "C++"s, "Java"s, "C"s, "C#"s};
    PrintRange(langs.begin(), langs.end());
}

int main() {
    TestIteratingOverStruct();
    TestIteratingOverStrings();
    return 0;
}
