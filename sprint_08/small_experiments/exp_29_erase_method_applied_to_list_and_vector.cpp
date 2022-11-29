#include <algorithm>
#include <iostream>
#include <list>
#include <ostream>
#include <set>
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

    // пробуем сделать то же самое, но с "std::list"
    // list<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s, "Go"s, "Ruby"s, "Haskel"s, "Cobol"s};

    // хотим удалить из вектора "langs" все языки программирования, начинающиеся с буквы 'C'
    // std::remove_if возвращат новый конец результирующего (после удалений) ветора
    // фактически std::remove_if ничего не удаляет, а лишь перемещает всё необходимое в начало вектора
    // и возвращает итератор, который будет являться новым past_the_end_iterator'ом результирующего контейнера

    // !!! В ЦЕЛОМ: общие алгоритмы, которые принимают итераторы, не могут менять размер контейнера!!!
    // auto new_past_the_end_iterator = std::remove_if(langs.begin(),
    //                                                 langs.end(),
    //                                                 [](const std::string& language) {
    //                                                     return language[0] == 'C';
    //                                                 });

    std::remove_if(langs.begin(),
                   langs.end(),
                   [](const std::string& language) {
                       return language[0] == 'C';
                   });

    // удаляем все элементы, начиная от "past_the_end_iterator" до конца оригинального вектора
    // langs.erase(new_past_the_end_iterator, langs.end());

    // проверяем, что получилось
    std::cout << "langs vector after deletion all languages starting from letter 'C':\n";
    PrintRange(langs.begin(), langs.end());
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;

    return 0;
}