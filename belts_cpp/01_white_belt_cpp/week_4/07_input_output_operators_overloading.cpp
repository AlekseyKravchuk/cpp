#include <iomanip>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>
#include <utility>  // std::move
#include <vector>

using namespace std::literals;

struct Duration {
    int hour{};
    int min{};
};

// перегружаем оператор ввода из потока, >> (stream extraction operator, "from") для объекта типа "Duration"
// 1-ым аргументом мы должны передать ссылку на поток, ИЗ КОТОРОГО мы будем читать,
// 2-ым аргументом мы должны передать ссылку (НЕконстантную) на объект, В КОТОРЫЙ мы будем записывать
std::istream& operator>>(std::istream& input_stream, Duration& duration) {
    input_stream >> duration.hour;
    input_stream.ignore();
    input_stream >> duration.min;

    // возвращаем ссылку на поток
    return input_stream;
}

// перегружаем оператор вывода в поток, << (stream insertion operator, "to") для объекта типа "Duration"
// оператор вывода в поток (<<) должен возвращать ссылку на поток, в который он осуществил вывод
// 1-ым аргументом мы должны передать ссылку на поток, В КОТОРЫЙ мы будем читать,
// 2-ым аргументом мы должны передать ссылку (константную) на объект, ИЗ КОТОРОГО мы будем считывать
std::ostream& operator<<(std::ostream& output_stream, const Duration& duration) {
    output_stream << std::setfill('0');
    output_stream << std::setw(2) << duration.hour << ":" << std::setw(2) << duration.min;

    return output_stream;
}

int main() {
    // создаем строковый поток для удобства (чтобы не вводить с консоли)
    std::stringstream dur_ss{"2 51"s};

    // создаем интервал - будет вызван генерируемый компилятором default constructor
    Duration dur_1;

    // читаем ИЗ строкового потока "dur_ss" с помощью перегруженного для структуры "Duration" оператора ввода
    dur_ss >> dur_1;
    // operator>>(dur_ss, dur_1);

    // выводим В поток "std::cout" с помощью перегруженного для структуры "Duration" оператора ввода
    std::cout << dur_1 << std::endl;
    // operator<<(operator<<(std::cout, dur_1), "\n");

    return 0;
}
