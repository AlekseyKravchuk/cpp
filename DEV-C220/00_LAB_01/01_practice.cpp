// Практика 1:
//  enum class
//  initializer_list и универсальные списки инициализации
//  auto
//  decltype
//  lambda
//  template variable

#include <cctype>  // std::toupper
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <type_traits>  // std::is_same
#include <vector>

#include "../my_print.h"

using namespace std::literals;

int main() {
    // Задание 1.
    {
        enum class months { January,
                            February,
                            March /*...*/ };
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
        // => НЕ ПОНЯЛ, что требуется сделать
        // months year[] = {"January"s, "February"s, "March"s};
        // size_t n = sizeof(year);
    }

    {
        /**********************************************************/
        // Задание 2. Создайте и с помощью списка инициализации
        // заполните значениями вектор с элементами - string
        // С помощью range-based for преобразуйте все буквы в
        // верхний регистр а остальные символы должны остаться
        // неизменными

        std::vector<std::string> words = {"a-ba"s, "c=aba"s, "ba12ba"s};
        for (auto& word : words) {
            for (auto& ch : word) {
                if (std::isalpha(ch)) {
                    ch = std::toupper(ch);
                }
            }
        }

        PrintCollection(words);

        /**********************************************************/
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
        std::map<char, std::vector<std::string>> mp = {
            {'A', {"any"s, "apple"s, "away"s}},
            {'B', {"some"s, "orange"s, "were"s, "eaten"s}},
            {'C', {"other"s, "means"s, "of"s, "programming"s}}};

        for (const auto& [character, words_collection] : mp) {
            std::cout << character << ": ";
            PrintCollection(words_collection);
        }
    }

    {
        // 3б. Создайте пустой map и используйте заданный массив
        // для выполнения задания.
        // С помощью range-based for и structured binding
        // распечатайте содержимое, например: A: any, apple, away

        // дано (например):
        const char* s[] = {"yong", "away", "bar", "any", "son", "apple"};
        std::map<char, std::vector<std::string>> mp;
        mp['A'] = std::vector<std::string>(s, s + std::size(s));

        for (const auto& [ch, str_vector] : mp) {
            std::cout << ch << ": "s;
            PrintCollection(str_vector);
        }
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