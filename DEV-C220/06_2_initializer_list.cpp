#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    std::vector<std::string> names = {"Dmitry"s, "Alex"s, "Oksana"s}; 

    // В С++20 есть возможность добавлять инициализатор
    // теперь помимо элемента есть возможность распечатать номер итерации (элемента)
    for (int i = 0; const auto& name : names) {
        std::cout << ++i << ": "s << name << std::endl;
    }

    // ПРИМЕЧАНИЕ!
    // для того, чтобы range-based for loop работал с пользовательской структурой данных:
    // 1) должны быть реализованы методы begin() и end()
    // 2) должен быть определен ИТЕРАТОР, для которого перегружены операторы *, !=, префиксный ++ (минимальный джентельменский набор)
}