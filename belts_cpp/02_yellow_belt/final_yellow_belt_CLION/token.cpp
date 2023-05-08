#include "token.h"

#include <stdexcept>

std::vector<Token> Tokenize(istream& is) {
    std::vector<Token> tokens;

    char c;
    while (is >> c) {
        if (isdigit(c)) {
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
