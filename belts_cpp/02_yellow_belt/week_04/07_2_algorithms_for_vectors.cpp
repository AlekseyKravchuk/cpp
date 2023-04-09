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
        {"Python"s, "C++"s, "C"s, "C++"s, "Java"s, "C++"s};

    // хотим удалить все повторы из вектора
    auto it = std::unique(langs.begin(), langs.end());
    langs.erase(it, langs.end());

    // проверяем содержимое вектора после работы std::unique
    std::cout << langs << std::endl;  // [Python, C++, C, C++, Java, C++]

    // Оказывыется, что "std::unique" удаляет только ВПОДРЯД ИДУЩИЕ повторения.
    // Таким образом, для того, чтобы оставить в векторе только уникальные элементы,
    // сначала его нужно отсторировать, а затем вызвать "std::unique" для этого отсортированного вектора:
    std::sort(langs.begin(), langs.end());
    langs.erase(std::unique(langs.begin(), langs.end()), langs.end());

    // проверяем содержимое вектора после работы "std::unique" и "std::vector::erase"
    std::cout << langs << std::endl;  // [C, C++, Java, Python] - получили ожидаемый результат

    return 0;
}
