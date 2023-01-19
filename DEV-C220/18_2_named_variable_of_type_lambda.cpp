#include <cmath>  // std::pow
#include <iostream>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    // Экземпляр структуры или класса можно создать, поименовать и дальше использовать.
    // То же самое можно сделать и с лямбдой.
    // В результате обработки правой части выражения компилятор сгенерирует анонимный класс или структуру
    // с перегруженным "operator(int x)";
    // далее вызывается конструктор по умолчанию для этого анонимного класса
    // и полученный объект (closure) сохранить в локальной переменной "raiseToSquare"
    // Вручную выписать тип такого неименованного объекта (closure) не представляется возможным.
    // Но компилятор умеет выводить тип такого объекта - даем компилятору инструкцию вывести тип самостоятельно, "auto"

    auto raiseToSquare = [](int x){ return std::pow(x, 2); }; // в это месте вызова перегруженного "operator() const" НЕТ
    int x = 8;
    // std::cout << raiseToSquare(x) << std::endl;
    std::cout << raiseToSquare.operator()(x) << std::endl;  // функциональная форма вызова перегруженного метода "operator() const"

    return 0;
}
