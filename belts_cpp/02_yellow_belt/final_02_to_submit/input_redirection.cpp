#include "input_redirection.h"

RedirectStandardInput::RedirectStandardInput(std::ifstream& input) {
    // сохраняем указатель на "streambuf"
    _cinbuf_bkp = std::cin.rdbuf();

    // перенаправляем поток ввода std::cin на файл
    std::cin.rdbuf(input.rdbuf());
}

RedirectStandardInput::~RedirectStandardInput() {
    std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
}
