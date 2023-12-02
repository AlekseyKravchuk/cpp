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
        {"Python"s, "C++"s, "C"s, "Java"s, "C#"s};

    // ищем элемент "C++"s
    auto it = std::find(std::begin(langs), std::end(langs), "C++"s);

    // хотим вставить вектор {"Haskel"s, "Go"s, "Kotlin"s} ПЕРЕД "C++"
    // вызов метода "std::vector::insert" от итератора и вставляемого значения (v.insert(it, value))
    // вставляет это значение прямо ПЕРЕД итератором "it"
    langs.insert(it, {"Haskel"s, "Go"s, "Kotlin"s});

    // проверяем содержимое вектора
    std::cout << langs << std::endl;  // [Python, Haskel, Go, Kotlin, C++, C, Java, C#]

    // если "it == std::begin(langs)", то вставка будет осуществлена в самое начало вектора
    langs.insert(langs.begin(), {"Javascript"s, "PHP"s, "R"s, "Swift"s});  // [Javascript, PHP, R, Swift, Python, Haskel, Go, Kotlin, C++, C, Java, C#]

    // проверяем содержимое вектора
    std::cout << langs << std::endl;  // [Python, Haskel, Go, Kotlin, C++, C, Java, C#]

    return 0;
}
