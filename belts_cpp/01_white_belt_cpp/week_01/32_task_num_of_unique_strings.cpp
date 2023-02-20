#include <fstream>
#include <iostream>
#include <string>
#include <set>

using namespace std::literals;

/*

Задание по программированию «Количество уникальных строк»

Дан набор строк. Найдите количество уникальных строк в этом наборе.

Формат ввода:
    - Сначала вводится количество строк N, затем — сами N строк, разделённые пробелом. Все строки состоят лишь из латинских букв, цифр и символов подчёркивания.

Формат вывода:
    - Выведите единственное целое число — количество уникальных строк в данном наборе.
 
Пример
Ввод:
6
first
second
first
third
second
second

Вывод:
3
*/

int main() {
    // std::ifstream in("32_task_input.txt");      // configuring input from the file "32_task_input.txt"
    // std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    // std::cin.rdbuf(in.rdbuf());                 // redirect std::cin to "32_task_input.txt"

    int n{};
    std::cin >> n;
    std::string str_holder{};
    std::set<std::string> uniques;

    for (int i = 0; i < n; ++i) {
        std::cin >> str_holder;
        uniques.insert(str_holder);
    }

    std::cout << uniques.size() << std::endl;

    // std::cin.rdbuf(cinbuf);  // reset to standard input again

    return 0;
}
