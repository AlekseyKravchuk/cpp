#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
/*
Часть вектора

Напишите функцию PrintVectorPart, принимающую вектор целых чисел numbers,
выполняющую поиск первого отрицательного числа в нём и выводящую в стандартный вывод все числа,
расположенные левее найденного, в обратном порядке.
Если вектор не содержит отрицательных чисел, выведите все числа в обратном порядке.

void PrintVectorPart(const vector<int>& numbers);

Пример кода

int main() {
  PrintVectorPart({6, 1, 8, -5, 4});
  cout << endl;
  PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
  cout << endl;
  PrintVectorPart({6, 1, 8, 5, 4});
  cout << endl;
  return 0;
}

Вывод:
8 1 6
4 5 8 1 6
*/

template <typename InputIterator>
void PrintRange(InputIterator it_begin, InputIterator it_end, const std::string& delim = " "s) {
    bool isFirst = true;
    for (auto it = it_begin; it != it_end; ++it) {
        if (!isFirst) {
            std::cout << delim;
        }
        isFirst = false;
        std::cout << *it;
    }
    std::cout << std::endl;
}

void PrintVectorPart(const std::vector<int>& numbers) {
    auto it = find_if(numbers.begin(), numbers.end(),
                      [](int value) {
                          return value < 0;
                      });
    if (it != numbers.end()) {
        bool isFirst = true;
        while (it != numbers.begin()) {
            --it;
            if (!isFirst) {
                std::cout << " "s;
            }
            isFirst = false;
            std::cout << *it;
        }
    } else {
        PrintRange(numbers.rbegin(), numbers.rend());
    }
}

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    cout << endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    cout << endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    cout << endl;
    return 0;
}