#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;
using std::string;
using std::vector;

// Для форматирования текста при выводе с С++ существуют
// ПОТОКОВЫЕ МАНИПУЛЯТОРЫ - это такие объекты, которые позволяют производить некоторые манипуляции с выводимыми в поток объектами

// std::setw - задать ширину поля вывода
// std::setprecision - задать точность вывода вещественных чисел (количество точек после запятой)
// std::setfill - задать заполнитель для std::setw
// std::left - задать выравнивание

void PrintNamesAndData(const vector<string>& names,
                       const vector<double>& values,
                       int columnWidt = 8) {
    std::cout << std::fixed << std::setprecision(4) << std::setfill('*') << std::right;

    for (const auto& name : names) {
        std::cout << std::setw(columnWidt) << name << " ";
    }
    std::cout << std::endl;

    for (const auto& val : values) {
        std::cout << std::setw(columnWidt) << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<std::string> names{"a"s, "b"s, "c"s, "d"s};
    std::vector<double> values{5, 0.01, 0.000005, 0.0987432};

    PrintNamesAndData(names, values, 10);

    return 0;
}
