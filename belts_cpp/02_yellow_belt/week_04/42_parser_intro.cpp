#include <iostream>
#include <memory>  // std::shared_ptr<T>
#include <string>

using namespace std::literals;

/*
Парсер будет заниматься разбором арифметического выражения, в котором могут встречаться:
    - цифры;
    - знаки '*', '+', '-'
    - переменная 'x', которая будет задаваться позднее и позволит вычислить значение получившегося выражения

Пример того, как должен работать разрабатываемый парсер:
    Enter expression: 1+2-3*x+x
    Enter x: 5
    Expression value: -7  // результат вычисления выражения 1+2-3*x+x при x=7 (1+2-3*5+5 = -7)
    Enter x: 1
    Expression value: 1   // результат вычисления выражения 1+2-3*x+x при x=1 (1+2-3*1+1 = 1)
*/

// пока непонятно, что из себя будет представлять тип объекта переменной "expr"
// пока только известно, что у типа, возвращаемого функцией "Parse", должен быть метод "Evaluate"
class Node {
   public:
    virtual int Evaluate() {
        return 0;
    }
};

class Digit : public Node {
   public:
    Digit(int d) : _d(d) {}

    int Evaluate() override {
        return _d;
    }

   private:
    int _d;
};

class Variable : public Node {
   public:
    Variable(const int& x) : _x(x) {}

    int Evaluate() override {
        return _x;
    }

   private:
    // сохраняем в классе КОНСТАНТНУЮ ссылку на некоторую внешнюю переменную
    const int& _x;
};

// слева и справа от знака операции может находиться, как число, так и цифра, поэтому
// нужно сохранять некоторое общее представление операнда => нужно придумать базовый класс для классов "Digit" и "Variable"
class Operation : public Node {
   public:
    Operation(char operation, std::shared_ptr<Node> left_operand, std::shared_ptr<Node> right_operand)
        : _operation(operation), _left_operand(left_operand), _right_operand(right_operand) {}

    int Evaluate() override {
        if (_operation == '*') {
            return _left_operand->Evaluate() * _right_operand->Evaluate();
        } else if (_operation == '+') {
            return _left_operand->Evaluate() + _right_operand->Evaluate();
        } else if (_operation == '-') {
            return _left_operand->Evaluate() - _right_operand->Evaluate();
        }
        return 0;  // если не распознали операцию, возвращаем ноль
    }

   private:
    const char _operation;
    std::shared_ptr<Node> _left_operand;
    std::shared_ptr<Node> _right_operand;
};

// пробуем написать функцию Parse - что она может возвращать:
// 1) объект типа Node
// 2) shared_ptr на этот объект Node
// Node Parse(const std::string& tokens, int& x) {
// }

int main() {
    std::string tokens = "5+7-x*x+x"s;
    // std::string tokens = "5"s;
    // std::string tokens = "x"s;
    // std::string tokens = "5+7*3"s;

    // std::string tokens{};
    // std::cout << "Enter expression: "s;
    // std::cin >> tokens >> std::ws;

    int x = 5;

    std::shared_ptr<Node> var1 = std::make_shared<Variable>(x);
    std::shared_ptr<Node> var2 = std::make_shared<Variable>(x);
    std::shared_ptr<Node> mul1 = std::make_shared<Operation>('*', var1, var2);

    std::shared_ptr<Node> dig1 = std::make_shared<Digit>(5);
    std::shared_ptr<Node> dig2 = std::make_shared<Digit>(7);
    std::shared_ptr<Node> plus1 = std::make_shared<Operation>('+', dig1, dig2);

    std::shared_ptr<Node> var3 = std::make_shared<Variable>(x);

    std::shared_ptr<Node> minus1 = std::make_shared<Operation>('-', plus1, mul1);

    std::shared_ptr<Node> plus2 = std::make_shared<Operation>('+', minus1, var3);



    std::cout << "Enter x: "s;

    // int x{};

    // // как бы мы хотели организовать наш парсер на уровне вызова конкретных методов?
    // // в функцию Parse передаем считанную строчку и значение "x"
    // auto expr = Parse(tokens, x);

    // // До тех пор, пока пользователь будет вводить значение переменной "x", будем вычислять значение данного арифм.выражения
    while (std::cin >> x) {
        // хотим вывести результат выражения
        std::cout << plus2->Evaluate() << std::endl;
        std::cout << "Enter x: ";
    }

    return 0;
}
