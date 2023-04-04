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

    for (std::vector<Lang>::iterator it = langs.begin(); it != langs.end(); ++it) {
        std::cout << *it << std::endl;
    }


    return 0;
}
