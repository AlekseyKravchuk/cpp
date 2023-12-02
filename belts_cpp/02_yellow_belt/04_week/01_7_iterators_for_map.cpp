#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std::literals;

// перегружаем (переопределеям) оператор вывода в поток для пары (std::pair)
std::ostream& operator<<(std::ostream& os, const std::pair<const std::string, int>& p) {
    os << "{"s << p.first << ", "s << p.second << "}"s;
    return os;
}

template <typename InputIterator>
void PrintRange(InputIterator it_begin, InputIterator it_end, const std::string& delim = " "s) {
    bool isFirst = true;
    for (auto it = it_begin; it != it_end; ++it) {
        if (!isFirst) {
            std::cout << delim;
        }
        isFirst = false;
        std::cout << *it;
    }
    std::cout << std::endl;
}

int main() {
    // Пусть в векторе лежат структуры
    std::map<std::string, int> langs =
        {{"Python"s, 26}, {"C++"s, 34}, {"C"s, 45}, {"Java"s, 22}, {"C#", 17}};

    PrintRange(langs.begin(), langs.end());

    auto it = langs.find("C++"s);

    PrintRange(langs.begin(), it);
    PrintRange(it, langs.end());

    return 0;
}
