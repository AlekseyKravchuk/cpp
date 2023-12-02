#include <fstream>
#include <iostream>
#include <string>

using namespace std::literals;

/*

Задание по программированию «Работа с файлами»

Часть 2

Снова считайте все содержимое файла input.txt, но на этот раз выведите его в файл output.txt . Точно так же гарантируется, что содержимое файла input.txt заканчивается переводом строки. 
Пример

input.txt

Keep calm
and
learn C++

output.txt

Keep calm
and
learn C++


*/

int main() {
    std::string input_path{"input.txt"s};
    std::ifstream input(input_path);

    std::string output_path{"output.txt"s};
    std::ofstream output(output_path);

    std::string line{};
    if (input) {
        while (std::getline(input, line)) {
            output << line << std::endl;
        }
    }

    return 0;
}
