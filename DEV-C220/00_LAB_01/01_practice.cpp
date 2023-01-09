// Практика 1:
//  enum class
//  initializer_list и универсальные списки инициализации
//  auto
//  decltype
//  lambda
//  template variable

#include "01_practice.h"

#include <cctype>  // std::toupper
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
// #include <map>
#include <string>
#include <type_traits>  // std::is_same
#include <vector>
// #include <set>

using namespace std::literals;

int main() {
    // Задание 1.
    {
        std::cout << "====================== Task #1 ======================"s << std::endl;

        enum class months : unsigned char { January,
                                            February,
                                            March /*...*/
        };

        enum class weekDays { Monday,
                              Tuesday,
                              Wednesday /*...*/ };

        months m = months::January;
        // а) уберите проблемы (это значит, что КОМПИЛЯТОР не
        // должен позволять программисту осуществлять опасные
        // сравнения!)

        if (m == months::February) { /*...*/
        }

        // б) оптимизируйте использование памяти

        months year[] = {months::January, months::February, months::March};
        size_t n = sizeof(year);
        std::cout << "size of months year[]: " << n << std::endl;

        std::cout << "=================== Task #1 ended ==================="s << std::endl
                  << std::endl;
    }

    {
        // Задание 2. Создайте и с помощью списка инициализации
        // заполните значениями вектор с элементами - string
        // С помощью range-based for преобразуйте все буквы в
        // верхний регистр а остальные символы должны остаться
        // неизменными

        std::cout << "====================== Task #2 ======================"s << std::endl;

        std::vector<std::string> words = {"a-ba"s, "c=aba"s, "ba12ba"s};
        for (auto& word : words) {
            for (auto& ch : word) {
                ch = std::toupper(ch);
            }
        }

        PrintCollection(words);

        std::cout << "=================== Task #2 ended ==================="s << std::endl
                  << std::endl;
    }

    {
        /// Задание 3. Создайте и заполните значениями
        // map двумя способами (3а и 3б) таким образом, чтобы он содержал
        // в качестве ключа букву, а в качестве значения
        // отсортированную по алфавиту совокупность слов (string),
        // начинающихся с этой буквы

        // 3а. Проинициализируйте map "вручную" посредством списка инициализации
        // С помощью range-based for и structured binding
        // распечатайте содержимое, например: A: any, apple, away

        std::cout << "====================== Task #3a ======================"s << std::endl;
        std::map<char, std::vector<std::string>> mp =
            {
                {'A', {"any"s, "apple"s, "away"s}},
                {'B', {"book"s, "boom"s, "bottom"s}},
                {'C', {"connection"s, "concise"s, "cesar"s}}};

        for (const auto& [character, words_collection] : mp) {
            std::cout << character << ": ";
            PrintCollection(words_collection);
        }
        std::cout << "=================== Task #3a ended ==================="s << std::endl
                  << std::endl;
    }

    {
        // 3б. Создайте пустой map и используйте заданный массив
        // для выполнения задания.
        // С помощью range-based for и structured binding
        // распечатайте содержимое, например: A: any, apple, away

        // дано (например):
        std::cout << "====================== Task #3b ======================"s << std::endl;
        const char* words[] = { "yong", "away", "Bar", "any", "son", "Apple", "West", "home"};
        std::map<char, std::set<std::string>> mp;

        for (const auto& word : words) {
            mp[std::toupper(word[0])].insert(word);
        }

        for (const auto& [ch, words] : mp) {
            std::cout << ch << ": "s;
            PrintCollection(words);
        }

        std::cout << "=================== Task #3b ended ==================="s << std::endl;
    }

    {
        // Задание 4. создать функцию для вывода на печать
        // элементов последовательностей, заданных ниже:
        std::cout << std::endl
                  << "******************** Task #4 ********************"s << std::endl;

        std::vector<double> vd = {1.1, 2.2, 3.3};
        // PrintAnyCont(vd);
        PrintCollection(vd);

        std::string s("abc");
        // PrintAnyCont(s);
        PrintCollection(s);

        int ar[] = {1, 2, 3};
        // PrintAnyCont(ar);
        PrintCollection(ar);

        std::initializer_list<int> il{3, 4, 5};
        // PrintAnyCont(il);
        PrintCollection(il);

        std::cout << std::endl
                  << "**************** Task #4 ended ****************"s << std::endl;
    }

    {
        /// Задание 5.
        // Cоздать функцию для "отрицания" значений, например:
        // было: {1, -2, 5}, стало: {-1, 2, -5})
        // изменение объектов типа std::string может выглядеть "aBc1" -> "AbC1"
        // элементов последовательностей, заданных ниже:

        std::cout << std::endl
                  << "******************** Task #5 ********************"s << std::endl;

        std::vector<double> vd{1.1, 2.2, 3.3};
        PrintCollection(vd, "Initial vd: "s);
        NegateAll(vd);
        PrintCollection(vd, "vd after applying NegateAll(vd): "s);
        std::cout << std::endl;

        // => КАК сделать специализацию для std::string?????
        // std::list<std::string> ls{"aBc", "Qwerty", "n12"};
        // PrintCollection(ls, "Initial ls: "s);
        // NegateAll(ls);
        // PrintCollection(ls, "ls after applying NegateAll(ls): "s);
        // std::cout << std::endl;

        // int ar[]{1, 2, 3};
        // PrintCollection(ar, "Initial ar: "s);
        // NegateAll(ar);
        // PrintCollection(ar, "ar after applying NegateAll(ar): "s);
        // std::cout << std::endl;

        std::cout << std::endl
                  << "**************** Task #5 ended ****************"s << std::endl;
    }
}