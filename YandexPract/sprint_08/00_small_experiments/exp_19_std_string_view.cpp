#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                // обратите внимание на это место в коде
                // здесь мы каждый раз копируем найденное слово
                // в новый элемент в векторе
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

// изменим название функции
// предыдущая её версия нам ещё пригодится

// пусть теперь наша функция возвращает вектор элементов string_view
vector<string_view> SplitIntoWordsView(const string& str) {
    vector<string_view> result;

    // 1
    int64_t pos = str.find_first_not_of(" ");

    // 2
    const int64_t pos_end = str.npos;

    // 3
    while (pos != pos_end) {
        // 4
        int64_t space = str.find(' ', pos);

        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));

        // 6
        pos = str.find_first_not_of(" ", space);
    }

    return result;
}

// создадим очень длинную строку,
// состоящую из слов из ста 'a'
string GenerateText() {
    const int SIZE = 10000000;
    string text(SIZE, 'a');
    for (int i = 100; i < SIZE; i += 100) {
        text[i] = ' ';
    }
    return text;
}

int main() {
    const string text = GenerateText();
    {
        LOG_DURATION("string");
        const auto words = SplitIntoWords(text);
        // выведем первое слово
        cout << words[0] << "\n";
    }
    {
        LOG_DURATION("string_view");
        const auto words = SplitIntoWordsView(text);
        // выведем первое слово
        cout << words[0] << "\n";
    }

    return 0;
}