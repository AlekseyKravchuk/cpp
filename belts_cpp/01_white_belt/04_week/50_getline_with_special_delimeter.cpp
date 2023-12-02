#include <fstream>  // std::ifstream <=> typedef basic_ifstream<char> ifstream;
#include <iostream>
#include <string>  // std::getline

using namespace std::literals;

int main() {
    std::string input_file_name = "50_date.txt"s;
    std::ifstream input(input_file_name);

    std::string year{};
    std::string month{};
    std::string day{};

    // проверяем, что со входным потоком можно работать
    if (input) {
        std::getline(input, year, '-');
        std::getline(input, month, '-');
        std::getline(input, day);

        std::cout << day << '.' << month << '.' << year << std::endl;
        
        std::cout << "Done." << std::endl;
    } else {
        std::cout << "Error while opening file: \"" << input_file_name << "\"." << std::endl;
    }

    return 0;
}
