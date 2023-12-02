#include <fstream>
#include <iostream>
#include <string>

using namespace std::literals;

/*
Задание по программированию «Работа с файлами»

В этом задании вам предстоит написать две небольшие программы. Каждый пункт - отдельная задача, решение отправляйте в поле с соответствующим номером.
Часть 1

Ваша программа должна считать содержимое файла input.txt и напечатать его на экран без изменений. Гарантируется, что содержимое файла input.txt заканчивается переводом строки.
Пример

input.txt:
Keep calm
and
learn C++

stdout:
Keep calm
and
learn C++


*/

int main() {
    std::string path{"input.txt"s};
    std::ifstream input(path);

    std::string line{};
    if (input) {
        while (std::getline(input, line)) {
            std::cout << line << std::endl;
        }
    }

    return 0;
}
