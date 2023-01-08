#include <fstream>
#include <iostream>
#include <string>

// функция, которая считывает весь файл
void ReadAll(const std::string& path) {
    using namespace std::literals;

    std::ifstream input(path);
    std::string line;

    // проверяем, что входной поток доступен
    if (input) {
        // построчно считываем файл и выводим его на экран
        while (std::getline(input, line)) {
            std::cout << line << std::endl;
        }
    } else {
        std::cout << "Opening a file for reading failed."s << std::endl;
    }
}

int main() {
    using namespace std::literals;

    const std::string path = "output.txt"s;

    // объявляем экземпляр класса "ofstream", т.к. мы будем ПИСАТЬ В файл
    std::ofstream output(path);

    // чтобы иметь возможность дозаписывать файл, а не переписывать его заново,
    // нужно при создании экземпляра "std::ofstream" добавить флажок "std::ios::app"
    // std::ofstream output(path, std::ios::app);

    // проверяем, что с нашим выходным потоком можно работать
    if (output) {
        // output << "hello"s << std::endl << "second line"s << std::endl << "third line"s << std::endl;
        output << "trying to append 4-th line"s << std::endl;
    } else {
        std::cout << "Opening a file \""s << path << "\" for writing failed."s << std::endl;
    }

    ReadAll(path);

    return 0;
}
