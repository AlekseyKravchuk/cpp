#include <iostream>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "../../log_duration.h"

std::vector<std::string> SplitIntoWords(const std::string& text) {
    std::vector<std::string> words;
    std::string current_word;

    for (const char ch : text) {
        if (ch == ' ') {
            // текущий символ является пробелом => сбрасываем всё, что накопили в "current_word" в наш вектор строк
            words.push_back(current_word);

            // зануляем текущее слово, т.к. мы его уже сохранили в векторе
            current_word.clear();
        } else {
            // символ не является пробелом => просто дописываем его к текущему слову с помощью метода std::string::push_back
            current_word.push_back(ch);
        }
    }

    // если последний символ в строке не является пробелом, то нам нужно как-то скинуть накопленное в "current_word" в вектор слов
    if (!current_word.empty()) {
        words.push_back(current_word);
    }

    return words;
}

std::vector<std::string> SplitIntoWordsEnhanced(const std::string& text) {
    std::vector<std::string> words;
    std::string current_word;

    for (const char ch : text) {
        if (ch == ' ') {
            // текущий символ является пробелом => сбрасываем всё, что накопили в "current_word" в наш вектор строк
            words.push_back(std::move(current_word));
            current_word.clear();
        } else {
            // символ не является пробелом => просто дописываем его к текущему слову с помощью метода std::string::push_back
            current_word.push_back(ch);
        }
    }

    // если последний символ в строке не является пробелом, то нам нужно как-то скинуть накопленное в "current_word" в вектор слов
    if (!current_word.empty()) {
        words.push_back(current_word);
    }

    return words;
}

std::string GenerateText() {
    const uint64_t SIZE = 1'000'000'000;
    const uint64_t WORD_LENGTH = 10'000'000;
    std::string text(SIZE, 'a');

    for (uint64_t i = WORD_LENGTH; i < SIZE; i += WORD_LENGTH) {
        text[i] = ' ';
    }

    return text;
}

int main() {
    // for (const std::string& word : SplitIntoWords("Red belt C++")) {
    //     std::cout << word << "\n";
    // }

    const std::string text = GenerateText();

    {
        LOG_DURATION("SplitIntoWords");
        SplitIntoWords(text);
    }

    {
        LOG_DURATION("SplitIntoWordsEnhanced");
        SplitIntoWordsEnhanced(text);
    }

    return 0;
}
