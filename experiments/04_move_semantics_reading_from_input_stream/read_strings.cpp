#include <istream>
#include <string>
#include <utility>
#include <vector>

#include "profile.h"

// Стоит задача считать набор строк из потока ввода и эти считанные строки положить в вектор.

std::vector<std::string> ReadStrings(std::istream& stream, bool use_move = false) {
    std::vector<std::string> strings;
    std::string s;

    while (stream >> s) {
        strings.push_back(use_move
                              ? std::move(s)
                              : s);
    }

    return strings;
}

// функция для нагрузочного тестирования
std::string GenerateText() {
    const int SIZE = 1'000'000'000;
    const int WORD_LENGTH = 10'000'000;
    std::string text(SIZE, 'a');

    for (int i = WORD_LENGTH; i < SIZE; i += WORD_LENGTH) {
        text[i] = ' ';
    }

    return text;
}

int main() {
    std::string text = GenerateText();
    std::istringstream iss1(text);
    std::istringstream iss2(text);

    {
        LOG_DURATION("ReadStrings");
        ReadStrings(iss1);
    }

    {
        LOG_DURATION("ReadStrings with move semantics");
        ReadStrings(iss2, true);
    }

    // ReadStrings: 735 ms
    // ReadStrings with move semantics: 708 ms

    return 0;
}
