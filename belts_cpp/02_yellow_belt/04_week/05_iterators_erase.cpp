#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std::literals;

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = " "s) {
    bool isFirst = true;
    std::ostringstream oss;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << delimeter;
        }
        isFirst = false;
        oss << elm;
    }
    return oss.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "["s << Join(v, ", "s) << "]"s;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{"s << Join(s, ", "s) << "}"s;
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    return os << "{"s << Join(m, ", "s) << "}"s;
}

int main() {
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "Java"s, "C#"};

    // ищем элемент "C++"s
    auto it = std::find(std::begin(langs), std::end(langs), "C++"s);

    // удаляем элемент по итератору
    langs.erase(it);

    // проверяем содержимое вектора
    std::cout << langs << std::endl;  // [Python, C, Java, C#]

    // вставляем "C++" обратно
    langs.insert(std::begin(langs), "C++"s);

    // проверяем содержимое вектора
    std::cout << langs << std::endl;  // [C++, Python, C, Java, C#]

    // ищем элемент "C"s
    it = std::find(std::begin(langs), std::end(langs), "C"s);

    // также можно удалить целый диапазон
    langs.erase(it, langs.end());

    // проверяем содержимое вектора
    std::cout << langs << std::endl;  // [C++, Python], был удален диапазон [C, Java, C#]

    return 0;
}
