#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
=================== Задание по программированию «Макрос UNIQ_ID» ===================
Разработать макрос UNIQ_ID, который будет формировать идентификатор,
уникальный в пределах данного cpp-файла. Например, следующий код должен компилироваться и работать:

int UNIQ_ID = 5;
string UNIQ_ID = "hello!";

В рамках данной задачи допускается, что код:
    int UNIQ_ID = 5; string UNIQ_ID = "hello"; // оба определения на одной строке
не будет компилироваться.
*/

#define TOKEN_PASTE(x, y) x##y
#define CAT(x, y) TOKEN_PASTE(x, y)
#define UNIQ_ID CAT(id_, __LINE__)

int main() {
      int UNIQ_ID = 0;
      std::string UNIQ_ID = "hello"s;
      std::vector<std::string> UNIQ_ID = {"hello"s, "world"s};
      std::vector<int> UNIQ_ID = {1, 2, 3, 4};
}