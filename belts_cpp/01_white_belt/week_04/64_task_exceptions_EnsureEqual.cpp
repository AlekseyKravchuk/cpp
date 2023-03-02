#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>  // std::numeric_limits<streamsize>::max()
#include <ostream>
#include <sstream>
#include <string>

using namespace std::literals;

/*
Задание по программированию «Функция EnsureEqual»

Напишите функцию void EnsureEqual(const string& left, const string& right);

В случае, если строка left не равна строке right,
функция должна выбрасывать исключение runtime_error с содержанием
"<l> != <r>", где <l> и <r> - строки, которые хранятся в переменных left и right соответственно.
Обратите внимание, что вокруг знака неравенства в строке, которая помещается в исключение, должно быть ровно по одному пробелу.

Если left == right, функция должна корректно завершаться.

Например, код:

int main() {
  try {
    EnsureEqual("C++ White", "C++ White");
    EnsureEqual("C++ White", "C++ Yellow");
  } catch (runtime_error& e) {
    cout << e.what() << endl;
  }
  return 0;
}

должен выводить

C++ White != C++ Yellow
*/

void EnsureEqual(const std::string& left, const std::string& right) {
    if(left.size() != right.size() || left != right) {
        std::ostringstream oss;
        oss << left << " != "s << right;
        throw std::runtime_error(oss.str());
    }
}

int main() {
    try {
        EnsureEqual("C++ White", "C++ White"s);
        EnsureEqual("C++ White", "C++ Yellow"s);
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
