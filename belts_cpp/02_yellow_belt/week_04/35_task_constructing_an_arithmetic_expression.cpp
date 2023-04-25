#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
Построение арифметического выражения
Часть 1

Реализуйте построение арифметического выражения согласно следующей схеме:
    - изначально есть выражение, состоящее из некоторого целого числа x;
    - на каждом шаге к текущему выражению применяется некоторая операция: прибавление числа, вычитание числа, умножение на число или деление на число; перед применением операции выражение всегда должно быть заключено в скобки.

Пример:
Изначально есть число 8, соответствующее выражение:
8

К нему применяется операция умножения на 3, получается выражение:
(8) * 3

Затем вычитается 6:
((8) * 3) - 6

Наконец, происходит деление на 1; итоговое выражение:
(((8) * 3) - 6) / 1

Формат ввода:
В первой строке содержится исходное целое число x. Во второй строке содержится целое неотрицательное число N— количество операций. В каждой из следующих N строк содержится очередная операция:
    - прибавление числа a: + a;
    - либо вычитание числа b: - b;
    - либо умножение на число c: * c;
    - либо деление на число d: / d.

Количество операций может быть нулевым — в этом случае необходимо вывести исходное число.

Формат вывода:
Выведите единственную строку — построенное арифметическое выражение.

Обратите внимание на расстановку пробелов вокруг символов:
    каждый символ бинарной операции (+, -, * или /) должен быть окружён ровно одним пробелом с каждой стороны: (8) * 3;
    символ унарного минуса (для отрицательных чисел) не нуждается в дополнительном пробеле: -5;
    скобки и числа не нуждаются в дополнительных пробелах.

Подсказка

Для преобразования числа к строке используйте функцию to_string из библиотеки <string>.
Пример
Ввод:
8
3
* 3
- 6
/ 1

Вывод:
(((8) * 3) - 6) / 1
*/

#ifdef _GLIBCXX_DEBUG
class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input) {
        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
#endif  //_GLIBCXX_DEBUG

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "35_input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    std::deque<std::string> q;
    std::string x{}, operation{}, operand{};
    int n_operations;

    std::cin >> x >> std::ws >> n_operations >> std::ws;
    q.push_back(x);

    for (auto i = 0; i < n_operations; ++i) {
        std::cin >> operation >> operand;
        q.push_front("(");
        q.push_back(")");
        q.push_back(" ");
        q.push_back(operation);
        q.push_back(" ");
        q.push_back(operand);
    }

    while (!q.empty()) {
        std::cout << q.front();
        q.pop_front();
    }

    std::cout << std::endl;

    return 0;
}
