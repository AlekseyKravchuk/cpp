#include <algorithm>
#include <iostream>
#include <iterator>  // std::inserter
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
        {"Python"s, "C++"s, "C"s, "C++"s, "Ada"s, "C#"s, "Java"s, "C++"s, "Scala"s};

    std::vector<std::string> c_langs;

    // хотим скопировать в вектор "c_langs" все ЯП, начинающиеся с буквы 'C'
    std::copy_if(langs.begin(), langs.end(),
                 std::back_inserter(c_langs),
                 [](const std::string& lang) {
                     return lang[0] == 'C';
                 });

    // выводим содержимое "c_langs"
    std::cout << c_langs << std::endl;

    // оставляем только уникальные элементы
    std::sort(c_langs.begin(), c_langs.end());
    c_langs.erase(std::unique(c_langs.begin(), c_langs.end()), c_langs.end());

    // выводим содержимое "c_langs"
    std::cout << c_langs << std::endl;

    return 0;
}
