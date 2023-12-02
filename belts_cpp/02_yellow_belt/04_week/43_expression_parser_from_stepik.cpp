#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// Внимание!
// Для простоты разбора будем использовать только числа из одной цифры,
// а также не будет скобок, пробелов и ненужных символов.
// При этом, будем считать, что выражение всегда корректно.
// Парсер основан на алгоритме "сортировочных станций" (Shunting yard algorithm)

struct Node {
    virtual int Evaluate() const = 0;
};

struct Value : public Node {
    Value(char digit) : _value(digit - '0') {}

    int Evaluate() const override { return _value; }

   private:
    const uint8_t _value;
};

struct Variable : public Node {
    Variable(const int &x) : _x(x) {}

    int Evaluate() const override { return _x; }

   private:
    const int &_x;
};

struct Op : public Node {
    Op(char value) : precedence([value]() {
                         if (value == '*') {
                             return 2;
                         } else {
                             return 1;
                         }
                     }()),
                     _op(value) {}

    const uint8_t precedence;

    int Evaluate() const override {
        if (_op == '*') {
            return _left->Evaluate() * _right->Evaluate();
        } else if (_op == '+') {
            return _left->Evaluate() + _right->Evaluate();
        } else if (_op == '-') {
            return _left->Evaluate() - _right->Evaluate();
        }

        return 0;
    }

    void SetLeft(std::shared_ptr<Node> node) { _left = node; }
    void SetRight(std::shared_ptr<Node> node) { _right = node; }

   private:
    const char _op;
    std::shared_ptr<const Node> _left;
    std::shared_ptr<const Node> _right;
};

template <class Iterator>
std::shared_ptr<Node> Parse(Iterator token, Iterator end, const int &x) {
    // Empty expression
    if (token == end) {
        return std::make_shared<Value>('0');
    }

    std::stack<std::shared_ptr<Node>> values;    // стек операндов
    std::stack<std::shared_ptr<Op>> operations;  // стек операций

    auto PopOps = [&](int precedence) {
        while (!operations.empty() && operations.top()->precedence >= precedence) {
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
        const auto &value = *token;
        if (value >= '0' && value <= '9') {
            values.push(std::make_shared<Value>(value));
        } else if (value == 'x') {
            values.push(std::make_shared<Variable>(x));
        } else if (value == '*') {
            PopOps(2);
            operations.push(std::make_shared<Op>(value));
        } else if (value == '+' || value == '-') {
            PopOps(1);
            operations.push(std::make_shared<Op>(value));
        }

        ++token;
    }

    while (!operations.empty()) {
        PopOps(0);
    }

    return values.top();
}

int main() {
    std::string tokens;
    std::cout << "Enter expression: ";
    std::getline(std::cin, tokens);

    int x = 0;
    auto node = Parse(tokens.begin(), tokens.end(), x);

    std::cout << "Enter x: ";
    while (std::cin >> x) {
        std::cout << "Expression value: " << node->Evaluate() << std::endl;
        std::cout << "Enter x: ";
    }

    return 0;
}