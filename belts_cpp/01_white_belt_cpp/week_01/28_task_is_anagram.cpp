#include <iostream>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

using namespace std::literals;

/*

Задание по программированию «Анаграммы»

Слова называются анаграммами друг друга, если одно из них можно получить перестановкой букв в другом.
Например, слово «eat» можно получить перестановкой букв слова «tea», поэтому эти слова являются анаграммами друг друга.
Даны пары слов, проверьте для каждой из них, являются ли слова этой пары анаграммами друг друга.

Указание:
    -Один из способов проверки того, являются ли слова анаграммами друг друга, заключается в следующем.
     Для каждого слова с помощью словаря подсчитаем, сколько раз в нём встречается каждая буква.
     Если для обоих слов эти словари равны (а это проверяется с помощью обычного оператора ==),
     то слова являются анаграммами друг друга, в противном случае не являются.

    - При этом построение такого словаря по слову удобно вынести в отдельную функцию BuildCharCounters.

Формат ввода:

Сначала дано число пар слов N, затем в N строках содержатся пары слов, которые необходимо проверить.
Гарантируется, что все слова состоят лишь из строчных латинских букв.

Формат вывода:

Выведите N строк: для каждой введённой пары слов YES, если эти слова являются анаграммами, и NO в противном случае.

Пример
Ввод:
3
eat tea
find search
master stream

Вывод:
YES
NO
YES
*/

std::map<char, int> BuildCharCounters(const std::string& word) {
    std::map<char, int> counter;
    for (const auto& ch : word) {
        ++counter[ch];
    }

    return counter;
}


int main() {
    int n{};
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string word1{}, word2{};
        std::cin >> word1 >> word2;
        if (BuildCharCounters(word1) == BuildCharCounters(word2)) {
            std::cout << "YES"s << std::endl;
        } else {
            std::cout << "NO"s << std::endl;
        }
    }
    return 0;
}
