#include <iostream>
#include <sstream>

#include "learner.h"
#include "log_duration.h"
#include "input_redirection.h"

#define _GLIBCXX_DEBUG 1  // включить режим отладки

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    Learner learner;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<std::string> words;
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            words.push_back(word);
        }
        
        {
            LOG_DURATION("Learn");
            std::cout << learner.Learn(words) << "\n";
        }
    }
    std::cout << "=== known words ===\n";

    {
        LOG_DURATION("KnownWords");
        for (auto word : learner.KnownWords()) {
            std::cout << word << "\n";
        }
    }
}  // по этой закрывающей скобке сработает деструктор ~RedirectStandardInput() и стандартный ввод будет восстановлен
