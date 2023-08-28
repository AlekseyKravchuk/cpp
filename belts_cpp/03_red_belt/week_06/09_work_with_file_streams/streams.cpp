#include <fstream>
#include <string>
#include <vector>

int main() {
    // хотим создать 5 файлов
    int num_files = 5;
    
    // создаем вектор файловых потоков
    std::vector<std::ofstream> streams;
    streams.reserve(num_files);

        for (int i = 0; i < num_files; ++i) {
        // на каждой итерации будем открывать поток
        std::ofstream stream("file_" + std::to_string(i) + ".txt");

        // выведем информацию в файл
        stream << "File #" << i << "\n";

        // и положим этот поток в вектор
        streams.push_back(std::move(stream));
    }

    // попробуем что-то вывести в наши файлы
    for (auto& stream : streams) {
        stream << "Vector is ready!" << std::endl;
    }

    return 0;
}
