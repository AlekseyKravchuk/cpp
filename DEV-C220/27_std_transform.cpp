#include <algorithm>  // std::transform
#include <functional>
#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <string>
#include <vector>

#include "my_print.h"

using namespace std::literals;

// The std::transform algorithm applies a UNARY(версия 1) or BINARY(версия 2) callable
// to a range and copies the modified elements to the destination range.
// ================ Version 1 (UNARY) ================
// Applies the UNARY callable fun to the elements of the input range
// and copies the result to "result", defined by output iterator (OutIt result):
// OutIt transform(InpIt first1, InpIt last1,
//                 OutIt result,
//                 UnaryFun fun);

// ================ Version 2 (BINARY) ================
// A second version of transform() allows a BINARY function to be applied to corresponding elements
// from two ranges specified by [InpIt1 first1, InpIt1 last1) and [InpIt2 first2, InpIt2 first2 + size(FirstRange))

// Applies the BINARY callable fun to both input ranges
// and copies the result to "result", defined by output iterator (OutIt result):
// OutIt transform(InpIt1 first1, InpIt1 last1,
//                 InpIt2 first2,
//                 OutIt result,
//                 BinaryFun fun)

int main() {
    // =============================== VERSION 1 (принимает 4 аргумента) ===============================
    {
        // Простой пример применения std::transform c унарной функцией,
        // результат записываем в новый контейнер
        std::vector<double> deg_Celsius{21.0, 30.5, 0.0, 3.2, 100.0};

        // создаем пустой контейнер и устанавливаем необходимую "capacity"
        // в этом случае третьим параметром "std::transform" нужно указывать std::back_inserter(TargetContainer)
        // std::vector<double> deg_Fahrenheit;
        // deg_Fahrenheit.reserve(deg_Celsius.size());

        std::vector<double> deg_Fahrenheit(deg_Celsius.size());
        PrintCollection(deg_Fahrenheit, "deg_Fahrenheit after calling ctor with desired size: "s);

        std::transform(deg_Celsius.begin(), deg_Celsius.end(),
                       deg_Fahrenheit.begin(),
                       [](double celsius) {
                           return (celsius * 1.8) + 32;
                       });

        // Store the results in an empty container by using a "std::back_insert_iterator" as the third argument to transform():
        // std::transform(deg_Celsius.begin(), deg_Celsius.end(),
        //                std::back_inserter(deg_Fahrenheit),
        //                [](double celsius) {
        //                    return (celsius * 1.8) + 32;
        //                });

        PrintCollection(deg_Fahrenheit, "deg_Fahrenheit after transforming: "s);
    }

    {
        // Простой пример применения std::transform c унарной функцией,
        // результат записываем в ИСХОДНЫЙ контейнер

        std::vector<double> deg_Celsius{21.0, 30.5, 0.0, 3.2, 100.0};

        // The third argument can be an iterator pointing to an element in the input container.
        // В этом случае результат применения унарной "callable", заданной в 4-ом параметре "std::transform"
        // возвращается в тот же элемент, к которому была применена унарная "callable".
        std::transform(deg_Celsius.begin(), deg_Celsius.end(),
                       deg_Celsius.begin(),
                       [](double celsius) {
                           return (celsius * 1.8) + 32;
                       });
        PrintCollection(deg_Celsius, "deg_Celsius after transforming IN-PLACE: "s);
    }

    {
        // Кроме того, возможна ситуация, когда ТИП элементов целевого диапазона (destination range, задается 3-им параметром)
        // отличается от типа элементов исходного диапазона (input range, задается 1-м и 2-м параметром std::transform)
        std::vector<std::string> words = {"one", "two", "three", "four", "five"};  // тип элемента - "const char*"
        std::vector<size_t> hash_values;                                           // тип элемента - "size_t"

        // поскольку целевой (destination) контейнер пустой, используем "std::back_inserter"
        std::transform(words.begin(), words.end(),
                       std::back_inserter(hash_values),
                       std::hash<std::string>());

        PrintCollection(hash_values);

        // распечатать содержимое контейнера можно с помощью алгоритма "std::copy" и потокового итератора (stream iterator)
        // The hashing function returns a hash value as type "size_t", and these values are stored in the "hash_values"
        // container using a "back_insert_iterator" object that is returned by the "std::back_inserter()" helper function
        // from the "iterator" header.
        // Для потокового итератора необходимо указать тип выводимых элементов: "size_t"
        std::copy(std::begin(hash_values), std::end(hash_values),
                  std::ostream_iterator<size_t>{std::cout, " "});
        std::cout << std::endl;

        // ВАЖНО !!!
        // Because the destination range is specified as a "back_insert_iterator" object, the transform() algorithm here would
        // return an iterator of type "back_insert_iterator<vector<size_T>>" so you could not use this as the end iterator for
        // the input range to the "copy()" algorithm.
        // To make use of the iterator that "std::transform" returns, the code would need to be:
        {
            std::vector<std::string> words = {"one", "two", "three", "four", "five"};
            std::vector<size_t> hash_values(words.size());

            // Now "std::transform" returns the end iterator for the range of elements in the hash_values container.
            auto end_iter = std::transform(words.begin(), words.end(),
                                           hash_values.begin(),
                                           std::hash<std::string>());

            // теперь "end_iter" можно использовать в качестве итератора в алгоритме "std::copy":
            std::copy(std::begin(hash_values), end_iter,
                      std::ostream_iterator<size_t>{std::cout, " == "});
            std::cout << std::endl
                      << std::endl;
        }

        // =============================== VERSION 2 (принимает 5 аргументов) ===============================
        // Количество вызовов БИНАРНОЙ функции (callable) определяется размером ПЕРВОГО ДИАПАЗОНА =>
        // размер второго диапазона, должен быть больше или равным размеру первого диапазона
        {
            {
                // размер первого контейнера меньше, чем у второго => алгоритм отработает корректно
                std::vector<int> first = {100, 200};
                std::vector<int> second = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
                std::vector<int> result(std::max(std::size(first), std::size(second)));

                std::transform(first.begin(), first.end(),
                               second.begin(),
                               result.begin(),
                               [](auto val_1, auto val_2) {
                                   return val_1 + val_2;
                               });

                std::copy(result.begin(), result.end(),
                          std::ostream_iterator<int>{std::cout, " "});
                std::cout << std::endl;
            }

            {
                // предполагается, что должно быть выполнено 12 операций,
                // задаваемых "callable" над соответствующими элементами контейнеров
                // НО!!! размера второго контейнера МЕНЬШЕ,
                // в результате 10 значений из 1-го контейнера будут складываться с "мусором"
                std::vector<int> first = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
                std::vector<int> second = {100, 200};
                std::vector<int> result(std::max(std::size(first), std::size(second)));

                std::transform(first.begin(), first.end(),
                               second.begin(),
                               result.begin(),
                               [](auto val_1, auto val_2) {
                                   return val_1 + val_2;
                               });
                
                // выводим полученный НЕКОРРЕКТНЫЙ результат
                std::copy(result.begin(), result.end(),
                          std::ostream_iterator<int>{std::cout, "-"});
                std::cout << std::endl;
            }
        }
    }

    return 0;
}
