#include <fstream>
#include <iostream>
#include <string>

int main() {
    using namespace std::literals;

    std::string fileToReadFrom = "date.txt"s;

    // объявляем экземпляр класса "ifstream", т.к. мы будем ЧИТАТЬ ИЗ файла
    std::ifstream input(fileToReadFrom);

    std::string year;
    std::string month;
    std::string day;

    // проверяем, что с нашим входным потоком можно работат
    if (input) {
        std::getline(input, year, '-');
        std::getline(input, month, '-');
        std::getline(input, day);
    } else {
        std::cout << "Opening a file \""s << fileToReadFrom << "\" for reading failed."s << std::endl;
    }

    std::cout << year << "-"s << month << "-"s << day << std::endl;

    return 0;
}
