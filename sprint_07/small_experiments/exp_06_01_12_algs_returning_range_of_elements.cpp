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

    // хотим разбить вектор "langs" на две части:
    // 1) языки программирования, начинающиеся с буквы 'C'
    // 2) все остальные ЯП

    vector<string>::iterator it = std::partition(langs.begin(),
                                                 langs.end(),
                                                 [](const std::string& language) {
                                                     return language[0] == 'C';
                                                 });
    // проверяем, что получилось
    std::cout << *it << std::endl;

    std::cout << "langs vector after partitioning: ";
    PrintRange(langs.begin(), langs.end());
    std::cout << std::endl;

    // Вначала идут ЯП, начинающиеся на букву 'C' (в каком-то порядке, как получилось), а потом все остальные ЯП
    // Все элементы, удовлетворяющие критерию, т.е. все те элементы, для которых лямбда-функция возвращает "true",
    // перемещаются в начало диапазона.
    // Причем "std::partition" возвращает итератор на следующий элемент после диапазона, для которого критерий вернул "true"
    // ==================================================================================
    // langs vector after partitioning: Cobol, C++, C, C#, Java, Go, Ruby, Haskel, Python
    // ==================================================================================

    // Теперь хотим отложить элементы, удовлетворяющие какому-то условию, но не в этот же вектор, а в какой-то другой.
    // Для этого можем воспользоваться алгоритмом std:copy_if.
    // Предварительно создаем целевой вектор и задаем ему размер с запасом
    vector<string> targetCLangs(langs.size());

    // В std::copy_if 3-им аргументом передаю КУДА мне нужно сохранить результат
    // в виде итератора на начало подготовленного нового вектора
    // std::copy_if возвращает итератор на новый конец
    auto itPastTheEndDesiredRange = std::copy_if(langs.begin(),
                                 langs.end(),
                                 targetCLangs.begin(),
                                 [](const std::string& language) {
                                     return language[0] == 'C';
                                 });
    // проверяем, что получилось
    // итератор "itPastTheEndDesiredRange" указывает на первый пустой элемент в результирующем веторе
    std::cout << "targetCLangs vector after copying langs starting from 'C' letter: ";
    PrintRange(targetCLangs.begin(), targetCLangs.end());
    std::cout << std::endl;

    // Оказывается, что поскольку мы зарезервировали в новом векторе место с запасом, у нас есть пустые элементы:
    // langs vector after copying langs starting from 'C' letter: Cobol, C++, C, C#, , , , ,
    // Избавимся от них:
    targetCLangs.erase(itPastTheEndDesiredRange, targetCLangs.end());

    // Смотрим, что получилось:
    std::cout << "targetCLangs vector after erasing empty elements: ";
    PrintRange(targetCLangs.begin(), targetCLangs.end());
    std::cout << std::endl;

    // Удостоверяемся, что размер "targetCLangs" изменился (уменьшился)
    std::cout << "New size of vector \"targetCLangs\" = "s << targetCLangs.size() << " elements."s << std::endl;
    
    return 0;
}