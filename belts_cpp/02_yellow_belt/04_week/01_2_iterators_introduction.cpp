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

struct Lang {
    std::string lang_name;
    int age;
};

std::ostream& operator<<(std::ostream& os, const Lang& lang) {
    os << "{name = "s << lang.lang_name << ", age = " << lang.age << "}"s;
    return os;
}

int main() {
    // Пусть в векторе лежат структуры
    std::vector<Lang> langs =
        {{"Python"s, 26}, {"C++"s, 34}, {"C"s, 45}, {"Java"s, 22}, {"C#", 17}};

    // Returns an iterator to the first element in the range [first,last) for which pred returns true.
    // If no such element is found, the function returns last.
    auto it = std::find_if(langs.begin(), langs.end(),
                           [](const Lang& lang) {
                               return lang.lang_name[0] == 'C';
                           });

    // оператор разыменования итератора "*(it)" возвращает неконстантную ССЫЛКУ на элемент коллекции
    if (it != langs.end()) {
        std::cout << *it << std::endl;
    } else {
        std::cout << "Not found"s << std::endl;
    }

    // можем поменять значение элемента в коллекции, поскольку разыменование итератора дает ССЫЛКУ на элемент
    if (it != langs.end()) {
        // (*it).lang_name = "DDD++"s;

        // существует более короткий синтаксис для обращения к полям
        it->lang_name = "DDD++"s;
    }

    // выводим вектор структур с измененным значением
    std::cout << Join(langs) << std::endl;

    return 0;
}
