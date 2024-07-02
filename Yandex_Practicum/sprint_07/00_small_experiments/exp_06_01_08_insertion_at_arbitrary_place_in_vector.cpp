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
    vector<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s};
    vector<string> langsToInsert = {"Go"s, "Ruby"s, "Haskel"s};

    // хотим вставить вектор "langsToInsert" в конец ветора "langs"
    // вызов "v.insert(it, value)" вставляет "value" прямо перед итератором "it"
    // в случае "v.insert(it, range_begin, range_end)" диапазон "[range_begin, range_end)" будет вставлен перед итератором "it"
    langs.insert(langs.end(), langsToInsert.begin(), langsToInsert.end());

    // проверяем, что получилось
    std::cout << "langs vector after insertion at the end: ";
    PrintRange(langs.begin(), langs.end());
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;


    return 0;
}