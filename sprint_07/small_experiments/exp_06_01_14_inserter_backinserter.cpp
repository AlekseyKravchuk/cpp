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
    
    // создаем вектор, в который будем копировать элементы, удовлетворяющие критерию
    vector<string> c_langs;

    // итератор "back_inserter" нужен исключительно для того, чтобы делать push_back в контейнер
    std::copy_if(begin(langs),
                 end(langs),
                 back_inserter(c_langs),
                 [](const string& lang){
                    return lang[0] == 'C';
                 });

    std::cout << "langs vector after std::copy_if being applied: ";
    PrintRange(c_langs.begin(), c_langs.end());
    std::cout << std::endl;
    std::cout << "size of c_langs = " << c_langs.size() << std::endl;
    
    return 0;
}