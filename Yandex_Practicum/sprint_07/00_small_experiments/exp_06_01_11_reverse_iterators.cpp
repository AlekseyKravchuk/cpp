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
    vector<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s, "Go"s, "Ruby"s, "Haskel"s, "Cobol"s};

    std::sort(langs.begin(), langs.end());
    std::cout << "langs vector after sorting: ";

    // "langs.begin()" возвращает тип "std::vector<string>::iterator"
    PrintRange(langs.begin(), langs.end());

    // =======================================================================
    // хотим вывести векто "langs" в обратном порядке
    std::cout << "langs vector in reverse order: ";

    // "langs.rbegin()" возвращает тип "std::vector<string>::reverse_iterator"
    PrintRange(langs.rbegin(), langs.rend());

    return 0;
}