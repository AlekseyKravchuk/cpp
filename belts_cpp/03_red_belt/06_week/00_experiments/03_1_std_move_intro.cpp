#include <iostream>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "../../log_duration.h"

std::vector<std::string> ReadStrings(std::istream& stream, bool use_std_move = false) {
    std::vector<std::string> strings;
    std::string s;

    while (stream >> s) {
        if (use_std_move) {
            strings.push_back(std::move(s));
        } else {
            strings.push_back(s);
        }
    }

    return strings;
}

// генерирует большой текст из 10 миллиардов символов и разбивает его пробелами через каждые 1 млрд. символов
std::string GenerateText() {
    const uint64_t SIZE = 10'000'000'000;
    const uint64_t WORD_LENGTH = 1'000'000'000;
    std::string text(SIZE, 'a');

    for (uint64_t i = WORD_LENGTH; i < SIZE; i += WORD_LENGTH) {
        text[i] = ' ';
    }

    return text;
}

int main() {
    const std::string text = GenerateText();

    {
        LOG_DURATION("WITHOUT std::move");
        std::istringstream iss(text);
        ReadStrings(iss);
    }

    {
        LOG_DURATION("with std::move");
        std::istringstream iss(text);
        ReadStrings(iss, true);
    }

    return 0;
}
