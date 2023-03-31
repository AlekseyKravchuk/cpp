#pragma once

#include <fstream>
#include <iostream>

using namespace std::literals;

class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input, std::string path) {
        input.open(path, std::ios::in);
        if (!input) {
            throw std::runtime_error("File \""s + path + "\" is not opened"s);
        }

        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
