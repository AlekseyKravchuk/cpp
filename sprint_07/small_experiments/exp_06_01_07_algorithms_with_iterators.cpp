#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    vector<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s, "Go"s, "Ruby"s, "Haskel"s};
    // initial state of vector "langs"
    std::cout << "initial state of langs vector: ";
    PrintRange(langs.begin(), langs.end());
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;

    // хотим в векторе "langs" найти элемент "C++" и удалить его
    auto cppBorderIt = std::find(langs.begin(), langs.end(), "C++"s);
    langs.erase(cppBorderIt);

    // проверяем содержимое langs после удаления элемента "C++
    std::cout << "langs vector after deletion of \"C++\": ";
    PrintRange(langs.begin(), langs.end());
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;

    // также с помощью метода std::vector<T,Allocator>::erase можно удалить нужный диапазон
    // например от "C++" до "Ruby" (не включая, т.е. "Ruby" в langs останется)
    auto rubyBorderIt = std::find(langs.begin(), langs.end(), "Ruby"s);
    langs.erase(cppBorderIt, rubyBorderIt);

    std::cout << "langs vector after deletion of range [\"C++\"; \"Ruby\"): ";
    PrintRange(langs.begin(), langs.end());
    std::cout << "==========================================" << std::endl;

    return 0;
}
