#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std::literals;

/*
Задание по программированию «Множество значений словаря»

Напишите функцию BuildMapValuesSet, принимающую на вход словарь map<int, string> и возвращающую множество значений этого словаря:
set<string> BuildMapValuesSet(const map<int, string>& m) {
  // ...
}

Пример:

Код:
set<string> values = BuildMapValuesSet({
    {1, "odd"},
    {2, "even"},
    {3, "odd"},
    {4, "even"},
    {5, "odd"}
});

for (const string& value : values) {
  cout << value << endl;
}

Вывод:
even
odd
*/

std::set<std::string> BuildMapValuesSet(const std::map<int, std::string>& m) {
    std::set<std::string> result_set;
    for (const auto& [key, value] : m) {
        result_set.insert(value);
    }

    return result_set;
}

int main() {
    // std::ifstream in("32_task_input.txt");      // configuring input from the file "32_task_input.txt"
    // std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    // std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "32_task_input.txt"

    std::set<std::string> values = BuildMapValuesSet(
        {{1, "odd"},
         {2, "even"},
         {3, "odd"},
         {4, "even"},
         {5, "odd"}});

    for (const std::string& value : values) {
        std::cout << value << std::endl;
    }

    // std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
