#include <fstream>  // std::ifstream <=> typedef basic_ifstream<char> ifstream;
#include <iomanip>
#include <iostream>
#include <string>  // std::getline
#include <vector>

using namespace std::literals;

void ReadFile(const std::string& path) {
    std::ifstream input(path);

    std::string s;
    if (input) {
        while (std::getline(input, s)) {
            std::cout << s << std::endl;
        }
    } else {
        std::cout << "Error while opening the file: \""s << path << "\"" << std::endl;
    }
}

void PrintData(const std::vector<std::string>& column_names,
               const std::vector<double>& values) {
    for (const auto& col : column_names) {
        std::cout << std::setw(10) << std::left << col << ' ';
    }

    std::cout << std::endl;

    for (const auto& value : values) {
        std::cout << std::fixed << std::setprecision(2) << std::left << std::setw(10) << value << ' ';
    }

    std::cout << std::endl;
}

int main() {
    // Рассмотрим задачу, когда нам нужно вывести данные (из файла) в некотором формате.
    // Например, названия каких-либо колонок и значения в этих колонках

    // Создадим вектор "names" с именами колонок
    std::vector<std::string> column_names{"a"s, "b"s, "c"s};

    // Создадим вектор значений
    std::vector<double> values{5, 0.01, 0.0000005};

    // выведем таблицу с названиями столбцов и данными
    PrintData(column_names, values);

    return 0;
}
