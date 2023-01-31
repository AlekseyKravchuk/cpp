// https://www.youtube.com/watch?v=ISVIrpT3AN8&list=PL6-8bkaLoP42NzUkQcTS4D8kNQC12xCC-&index=9
#include <exception>  // std::exception
#include <iostream>
#include <memory>  // std::shared_ptr
#include <string>

using namespace std::literals;

// TO DO: изучить алгоритм "shunting yard" для решения задачи

class Node {
   public:
    // вычисление значения выражения
    virtual int Evaluate() = 0;
};

class Value : public Node {
   public:
    Value(char digit) : _value(digit - '0') {}

    int Evaluate() override { return _value; }

   private:
    const int _value{};
};

class Variable : public Node {
   public:
    Variable(const int& x) : _x(x) {}

    int Evaluate() override { return _x; }

   private:
    const int& _x;
};

class Operation : public Node {
   public:
    Operation(char op, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : _op(op), _left(left), _right(right) {}

    int Evaluate() override {
        if (_op == '*') {
            return _left->Evaluate() * _right->Evaluate();
        } else if (_op == '+') {
            return _left->Evaluate() + _right->Evaluate();
        } else if (_op == '-') {
            return _left->Evaluate() - _right->Evaluate();
        } else if (_op == '/') {
            return _left->Evaluate() / _right->Evaluate();
        } else {
            throw std::invalid_argument("Invalid operation."s);
        }
    }

   private:
    const char _op;
    std::shared_ptr<Node> _left;
    std::shared_ptr<Node> _right;
};

// функция, которая разбирает выражение
// возвращает как сам объект "Node", так и "shared_ptr<Node>"
// Node Parse(const std::string& tokens, int& x) {
// }

int main() {
    std::string tokens = "5 + 7 - x*x + x"s;
    int x{1};

    std::shared_ptr<Node> var1 = std::make_shared<Variable>(x);
    std::shared_ptr<Node> var2 = std::make_shared<Variable>(x);
    std::shared_ptr<Node> mul1 = std::make_shared<Operation>('*', var1, var2);

    std::shared_ptr<Node> dig1 = std::make_shared<Value>(5);
    std::shared_ptr<Node> dig2 = std::make_shared<Value>(7);
    std::shared_ptr<Node> plus1 = std::make_shared<Operation>('+', dig1, dig2);

    std::shared_ptr<Node> minus1 = std::make_shared<Operation>('-', plus1, mul1);

    std::shared_ptr<Node> var3 = std::make_shared<Variable>(x);

    std::shared_ptr<Node> plus2 = std::make_shared<Operation>('+', minus1, var3);
    
    std::cout << "5 + 7 - x*x + x [при x = "s << x << "] = "s << plus2->Evaluate() << std::endl;

    return 0;
}
