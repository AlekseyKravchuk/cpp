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
    std::vector<std::string> langs =
        {"Python"s, "C++"s, "C"s, "Java"s, "C#"};

    PrintRange(langs.begin(), langs.end(), ", "s);

    // шаблонная функция позволяет выводить содержимое разных коллекций
    PrintRange(langs[3].begin(), langs[3].end(), ", "s);

    return 0;
}
