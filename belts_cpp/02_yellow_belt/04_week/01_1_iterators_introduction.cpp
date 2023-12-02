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

int main() {
    // Хотим найти в вектора "langs" какой-то ЯП, который начинается с буквы 'C'
    // или сказать, что такого языка в векторе нет
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "Java"s, "C#"};

    // Returns an iterator to the first element in the range [first,last) for which pred returns true.
    // If no such element is found, the function returns last.
    auto it = std::find_if(langs.begin(), langs.end(),
                           [](const std::string& lang) {
                               return lang[0] == 'C';
                           });

    // оператор разыменования итератора "*(it)" возвращает неконстантную ССЫЛКУ на элемент коллекции
    std::cout << ((it != langs.end()) ? *it : "Not found"s) << std::endl;

    // можем поменять значение элемента в коллекции, поскольку разыменование итератора дает ССЫЛКУ на элемент
    if (it != langs.end()) {
        *it = "DDD++";
    }

    std::cout << Join(langs) << std::endl;

    return 0;
}
