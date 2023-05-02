#include <cctype>    // std::isdigit
#include <iostream>  // std::istream::get
#include <sstream>
#include <stdexcept>
#include <vector>

#include "00_print.h"

enum class TokenType {
    DATE,
    EVENT,
    COLUMN,
    LOGICAL_OP,
    COMPARE_OP,
    PAREN_LEFT,
    PAREN_RIGHT,
};

struct Token {
    const std::string value;
    const TokenType type;
};

std::ostream& operator<<(std::ostream& os, const TokenType type) {
    switch (type) {
        case TokenType::DATE:
            os << "DATE"s;
            break;
        case TokenType::EVENT:
            os << "EVENT"s;
            break;
        case TokenType::COLUMN:
            os << "COLUMN"s;
            break;
        case TokenType::LOGICAL_OP:
            os << "LOGICAL_OP"s;
            break;
        case TokenType::COMPARE_OP:
            os << "COMPARE_OP"s;
            break;
        case TokenType::PAREN_LEFT:
            os << "PAREN_LEFT"s;
            break;
        case TokenType::PAREN_RIGHT:
            os << "PAREN_RIGHT"s;
            break;

        default:
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "["s << token.value << ", "s << token.type << "]"s << std::endl;
    return os;
}

std::vector<Token> Tokenize(std::istream& is) {
    std::vector<Token> tokens;

    char c;
    while (is >> c) {
        if (std::isdigit(c)) {
            std::string date(1, c);
            for (int i = 0; i < 3; ++i) {
                while (isdigit(is.peek())) {
                    date += is.get();
                }
                if (i < 2) {
                    date += is.get();  // Consume '-'
                }
            }
            tokens.push_back({date, TokenType::DATE});
        } else if (c == '"') {
            std::string event;
            std::getline(is, event, '"');
            tokens.push_back({event, TokenType::EVENT});
        } else if (c == 'd') {
            if (is.get() == 'a' && is.get() == 't' && is.get() == 'e') {
                tokens.push_back({"date", TokenType::COLUMN});
            } else {
                throw std::logic_error("Unknown token");
            }
        } else if (c == 'e') {
            if (is.get() == 'v' && is.get() == 'e' && is.get() == 'n' &&
                is.get() == 't') {
                tokens.push_back({"event", TokenType::COLUMN});
            } else {
                throw std::logic_error("Unknown token");
            }
        } else if (c == 'A') {
            if (is.get() == 'N' && is.get() == 'D') {
                tokens.push_back({"AND", TokenType::LOGICAL_OP});
            } else {
                throw std::logic_error("Unknown token");
            }
        } else if (c == 'O') {
            if (is.get() == 'R') {
                tokens.push_back({"OR", TokenType::LOGICAL_OP});
            } else {
                throw std::logic_error("Unknown token");
            }
        } else if (c == '(') {
            tokens.push_back({"(", TokenType::PAREN_LEFT});
        } else if (c == ')') {
            tokens.push_back({")", TokenType::PAREN_RIGHT});
        } else if (c == '<') {
            if (is.peek() == '=') {
                is.get();
                tokens.push_back({"<=", TokenType::COMPARE_OP});
            } else {
                tokens.push_back({"<", TokenType::COMPARE_OP});
            }
        } else if (c == '>') {
            if (is.peek() == '=') {
                is.get();
                tokens.push_back({">=", TokenType::COMPARE_OP});
            } else {
                tokens.push_back({">", TokenType::COMPARE_OP});
            }
        } else if (c == '=') {
            if (is.get() == '=') {
                tokens.push_back({"==", TokenType::COMPARE_OP});
            } else {
                throw std::logic_error("Unknown token");
            }
        } else if (c == '!') {
            if (is.get() == '=') {
                tokens.push_back({"!=", TokenType::COMPARE_OP});
            } else {
                throw std::logic_error("Unknown token");
            }
        }
    }

    return tokens;
}

int main() {
    std::string whole = "Find date >= 2017-01-01 AND date < 2017-07-01 AND event == \"sport event\" "s;
    std::istringstream iss(whole);
    std::string command{};
    iss >> command;
    std::cout << "Command = "s << command << std::endl;
    std::vector<Token> res = Tokenize(iss >> std::ws);
    std::cout << res << std::endl;
    return 0;
}
