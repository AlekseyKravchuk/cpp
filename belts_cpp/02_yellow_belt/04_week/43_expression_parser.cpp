#include <cctype>  // std::isdigit
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std::literals;

// Внимание!
// Для простоты разбора будем использовать только числа из одной цифры, а также не будет скобок, пробелов и ненужных символов.
// При этом, будем считать, что выражение всегда корректно. Парсер основан на алгоритме "сортировочных станций" (Shunting yard algorithm)
 
struct Node {
    virtual int Evaluate() const = 0;
};

struct Value : public Node {
    Value(char digit) : _value(static_cast<uint8_t>(digit - '0')) {}

    int Evaluate() const override { return _value; }

   private:
    const uint8_t _value;
};

struct Variable : public Node {
    Variable(const int &x) : _x(x) {}

    int Evaluate() const override { return _x; }

   private:
    const int& _x;
};

struct Operation : public Node {
    Operation(char value)
        : _precedence([value]() {
              return (value == '*') ? uint8_t{2} : uint8_t{1};
          }()),
          _operation(value) {}

    int Evaluate() const override {
        if (_operation == '*') {
            return _left->Evaluate() * _right->Evaluate();
        } else if (_operation == '+') {
            return _left->Evaluate() + _right->Evaluate();
        } else if (_operation == '-') {
            return _left->Evaluate() - _right->Evaluate();
        }

        return 0;
    }

    void SetLeft(std::shared_ptr<Node> node) { _left = node; }
    void SetRight(std::shared_ptr<Node> node) { _right = node; }

   public:
    const uint8_t _precedence;

   private:
    const char _operation;
    std::shared_ptr<const Node> _left;
    std::shared_ptr<const Node> _right;
};

template <typename Iterator>
std::shared_ptr<Node> Parse(Iterator token, Iterator end, const int& x) {
    if (token == end) {  // Empty expression
        return std::make_shared<Value>('0');
    }

    std::stack<std::shared_ptr<Node>> values;           // стек со значениями (с операндами арифметических выражений)
    std::stack<std::shared_ptr<Operation>> operations;  // стек операций

    auto PopOps = [&](int precedence) {
        while (!operations.empty() && operations.top()->_precedence >= precedence) {
            auto value1 = values.top();
            values.pop();
            auto value2 = values.top();
            values.pop();
            auto op = operations.top();
            operations.pop();

            op->SetRight(value1);
            op->SetLeft(value2);

            values.push(op);
        }
    };

    while (token != end) {
        const char &value = *token;
        if (std::isdigit(value)) {
            values.push(std::make_shared<Value>(value));
        } else if (value == 'x') {
            values.push(std::make_shared<Variable>(x));
        } else if (value == '*') {
            PopOps(2);
            operations.push(std::make_shared<Operation>(value));
        } else if (value == '+' || value == '-') {
            PopOps(1);
            operations.push(std::make_shared<Operation>(value));
        }

        ++token;
    }

    while (!operations.empty()) {
        PopOps(0);
    }

    return values.top();
}

int main() {
    std::string tokens = "2+3*x"s;
    // std::string tokens = "1"s;
    int x = 15;

    std::shared_ptr<Node> node = Parse(tokens.begin(), tokens.end(), x);
    std::cout << "Expression value: " << node->Evaluate() << std::endl;

    return 0;
}