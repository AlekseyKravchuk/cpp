#include <fstream>  // std::ifstream <=> typedef basic_ifstream<char> ifstream;
#include <iomanip>
#include <iostream>
#include <string>  // std::getline
#include <vector>

using namespace std::literals;

void ReadFile(const std::string& path) {
    std::ifstream input(path);

    std::string s;
    if (input) {
        while (std::getline(input, s)) {
            std::cout << s << std::endl;
        }
    } else {
        std::cout << "Error while opening the file: \""s << path << "\"" << std::endl;
    }
}

int main() {
    const std::string path = "52_output.txt"s;
    std::ofstream output(path, std::ios::app);

    // запишем в файл несколько тестовых строк
    output << "Hello"s << std::endl;
    output << "world"s << std::endl;
    output << "test lines"s << std::endl;

    // прочитаем, что записалось в файл
    ReadFile(path);

    return 0;
}
