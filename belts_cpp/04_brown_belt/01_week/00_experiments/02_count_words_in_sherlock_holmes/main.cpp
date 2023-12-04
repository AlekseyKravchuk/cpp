#include <algorithm>
#include <fstream>
#include <functional> // std::greater
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "iterator_range.h"
#include "profile.h"

std::vector<std::string> SplitIntoWords(const std::string& str) {
    std::vector<std::string> result;
    size_t nonspace_pos{0}, pos{0};

    while (true) {
        nonspace_pos = str.find_first_not_of(' ', pos);
        if (nonspace_pos == std::string::npos) {
            break;
        }

        pos = str.find_first_of(' ', nonspace_pos);
        result.push_back(
            pos == std::string::npos
                ? str.substr(nonspace_pos)
                : str.substr(nonspace_pos, pos - nonspace_pos));

        nonspace_pos = pos;
    }

    return result;
}

int main() {
    // std::map<std::string, size_t /* , std::greater<std::string> */> freqs;
    std::unordered_map<std::string, size_t /* , std::greater<std::string> */> freqs;

    {
        // Этап #1 - заполнение контейнера словами из файла
        LOG_DURATION("Fill");
        std::fstream fs("adventures_of_sherlock_holmes.txt");

        for (std::string line; std::getline(fs, line);) {
            for (const auto& word : SplitIntoWords(line)) {
                ++freqs[word];
            }
        }
    }

    // Этап #2 - заполнение вектора копиями из словаря (std::map) и сортировка этого вектора
    LOG_DURATION("Copy into vector and sorting");

    std::vector<std::pair<std::string, size_t>> words(freqs.begin(), freqs.end());
    std::sort(words.begin(),
              words.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs.second > rhs.second;
              });

    for (const auto& [word, count] : Head(words, 10)) {
        std::cout << word << '\t' << count << '\n';
    }
    std::cout << std::endl;

    return 0;
}
