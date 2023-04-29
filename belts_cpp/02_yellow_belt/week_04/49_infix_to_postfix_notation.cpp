#include <cctype>  // std::isdigit
#include <deque>
#include <iostream>
#include <map>
#include <sstream>  // std::ostringstream
#include <stack>
#include <stdexcept>
#include <string>

using namespace std::literals;

const std::map<char, uint8_t> priority = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2}};

bool isOperator(const char token) {
    return static_cast<bool>(priority.count(token));
}

void processToken(const char token, std::deque<char>& postfix, std::stack<char>& op_stack) {
    // 1) токен может быть числом
    if (std::isdigit(token)) {
        postfix.push_back(token);

    } else if (isOperator(token)) {  // 2) токен может быть оператором
        // Пока на вершине стека "operator_stack" лежит оператор с бОльшим или равным приоритетом, чем текущий "token",
        // перемещаем операторы из стека "operator_stack" в конец очереди "postfix".
        while (!op_stack.empty() &&
               op_stack.top() != '(' &&
               priority.at(op_stack.top()) >= priority.at(token)) {
            postfix.push_back(op_stack.top());
            op_stack.pop();
        }

        // После того, как проблема согласования приоритетов операций решена, помещаем текущий токен в стек "operator_stack".
        op_stack.push(token);

    } else if (token == '(') {  // 3) токен может быть левой скобкой (открывающей)
        op_stack.push(token);

    } else if (token == ')') {  // 4) токен может быть правой скобкой (закрывающей)

        // пока не доберемся до левой (открывающей) скобки в стеке "operator_stack",
        // перемещаем операторы с вершины стека "operator_stack" в конец очереди "postfix".
        while (op_stack.top() != '(') {
            postfix.push_back(op_stack.top());
            op_stack.pop();
        }

        // остается удалить текущую левую (открывающую) скобку
        if (!op_stack.empty()) {
            op_stack.pop();
        }
    } else {
        std::ostringstream oss;
        oss << "Unknown operator appeared: '"s << token << "'"s;
        throw std::runtime_error(oss.str());
    }
}

std::string toPostfix(const std::string& infix_expression) {
    std::deque<char> postfix;
    std::stack<char> op_stack;

    for (const auto& token : infix_expression) {
        processToken(token, postfix, op_stack);
    }

    // после того, как мы обработали инфиксное выражение (infix_expression),
    // необходимо очистить стек операторов - переместить содержимое стека в конец "postfix" в порядке извлечения из стека (LIFO)
    while (!op_stack.empty()) {
        postfix.push_back(op_stack.top());
        op_stack.pop();
    }

    return std::string(postfix.begin(), postfix.end());
}

double EvaluatePostfixExpression(const std::string& postfix_str) {
    std::stack<double> numbers;

    for (const auto& token : postfix_str) {
        if (std::isdigit(token)) {
            // since the character '0' has the ASCII code of 48, you can just write:
            numbers.push(token - '0');
            continue;
        }

        // дошли до первого оператора
        auto right = numbers.top();
        numbers.pop();

        auto left = numbers.top();
        numbers.pop();

        switch (token) {
            case '+':
                numbers.push(left + right);
                break;
            case '-':
                numbers.push(left - right);
                break;
            case '*':
                numbers.push(left * right);
                break;
            case '/':
                numbers.push(left / right);
                break;
            default: {
                std::ostringstream oss;
                oss << "Unknown operator appeared: '"s << token << "'"s;
                throw std::runtime_error(oss.str());
                break;
            }
        }
    }

    return numbers.top();
}

int main() {
    // std::string infix = "2+3"s;
    std::string infix = "(5+7)/2"s;
    // std::string infix = "5+7/2"s;
    // std::string infix = "5*2-3"s;
    // std::string infix = "5*(2-3)"s;
    // std::string infix = "2+(3*(8-4))"s;
    // std::cout << toPostfix(infix) << std::endl;

    // std::string infix = "2+(3*(8-4))"s;
    // std::string infix = "2+3"s;
    std::string postfix_str = toPostfix(infix);
    std::cout << EvaluatePostfixExpression(postfix_str) << std::endl;

    return 0;
}
