#include <iostream>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "../../log_duration.h"

std::vector<std::string> ReadStrings(std::istream& stream, bool use_std_move = false) {
    std::vector<std::string> strings;
    std::string s;

    // Как можно заметить разницу с "std::move" и без "std::move"?
    // Это можно заметить даже не по ускорению, а по тому, что осталось в переменной "s"

    while (stream >> s) {
        // std::cout << "before push_back: s = " << s << '\n';

        if (use_std_move) {
            strings.push_back(std::move(s)); // метод "push_back" забирает данные у строки
            // std::cout << "--- moving ---" << std::endl;
        } else {
            strings.push_back(s);
            // std::cout << "--- copying ---" << std::endl;
        }
        // std::cout << "AFTER push_back: s = " << s
        //           << ", strings.back() = " << strings.back() << std::endl;
        // std::cout << "============================================" << std::endl;
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
    // std::istringstream iss("Red belt C++");
    // for (const std::string& s : ReadStrings(iss, true)) {
    //     std::cout << s << std::endl;
    // }

    // сохраним сгенерированный текст
    const std::string text = GenerateText();

    {
        LOG_DURATION("without std::move");
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
