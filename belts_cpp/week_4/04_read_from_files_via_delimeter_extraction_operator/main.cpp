#include <fstream>
#include <iostream>
#include <string>

int main() {
    using namespace std::literals;

    std::string fileToReadFrom = "date.txt"s;

    // объявляем экземпляр класса "ifstream", т.к. мы будем ЧИТАТЬ ИЗ файла
    std::ifstream input(fileToReadFrom);

    int year{0};
    int month{0};
    int day{0};

    // проверяем, что с нашим входным потоком можно работать
    if (input) {
        input >> year;

        // теперь в потоке у нас первым идет символ минус '-' => нужно проигнорировать следующий 1 символ:
        input.ignore(1);

        input >> month;
        input.ignore(1);

        input >> day;
        input.ignore(1);

    } else {
        std::cout << "Opening a file \""s << fileToReadFrom << "\" for reading failed."s << std::endl;
    }

    std::cout << year << "-"s << month << "-"s << day << std::endl;

    return 0;
}
