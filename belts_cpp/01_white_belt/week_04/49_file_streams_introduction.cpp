#include <fstream>  // std::ifstream <=> typedef basic_ifstream<char> ifstream;
#include <iostream>
#include <string>  // std::getline

using namespace std::literals;

int main() {
    std::string input_file_name = "49_hello.txt"s;
    // std::string input_file_name = "50_hello.txt"s;
    std::ifstream input(input_file_name);

    if (input) {
        std::string line{};

        // "std::getline" читает данные из потока до разделителя, по умолчанию в качестве разделителя используется перевод строки '\n'
        // !!! При этом перевод строки '\n' в результирующую строку "line" НЕ ПОПАДАЕТ!!!
        while(std::getline(input, line)) {
            std::cout << line << std::endl;
        }

        std::cout << "Done." << std::endl;
    } else {
        std::cout << "Error while opening file: \"" << input_file_name << "\"." << std::endl;
    }

    return 0;
}
