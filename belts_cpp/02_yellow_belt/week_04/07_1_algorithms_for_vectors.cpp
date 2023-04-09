#include <algorithm>
#include <iostream>
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

int main() {
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "Java"s, "C#"};

    // хотим удалить из вектора все языки, начинающиеся с буквы 'C'
    auto it = std::remove_if(langs.begin(), langs.end(),
                             [](const std::string& s) {
                                 return s[0] == 'C';
                             });

    // проверяем содержимое вектора
    std::cout << langs << std::endl;

    // Смотрим, на какой элемент вектора указывает итератор, который нам вернул алгоритм "std::remove_if".
    // Итератор указывает на элемент, который следует сразу за последним неудаленным элементом (указывает на новый конец вектора).
    std::cout << "Iterator points to the element: "s << *it << std::endl;

    // удаляем ненужный мусор в векторе
    langs.erase(it, langs.end());

    // снова выводим вектор, но уже после ФАКТИЧЕСКОГО удаления ненужных элементов
    std::cout << langs << std::endl;

    return 0;
}
