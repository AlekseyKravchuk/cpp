#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "log_duration.h"

int main() {
    // поток вывода "std::ofstream" буферизирует выводимиые в него данные в некотором внутреннем буфере
    // и в определенный момент "std::ofstream" сбрасывает буфер в файл

    // когда мы используем манипулятор "std::endl", он не только вставляет в поток символ перевода строки,
    // но и сбрасывает его буфер, т.е. текущий накопленный буфер потока сбрасывается в файл
    int repetitions_count = 150'000;
    {
        LOG_DURATION("std::endl"s);
        std::ofstream out("output_1.txt"s);
        for (int i = 0; i < repetitions_count; ++i) {
            out << "London is the capital of Great Britain. "s
                << "I am travelling down the river."s
                << std::endl;
        }
    }

    {
        LOG_DURATION("'\\n'"s);
        std::ofstream out("output_2.txt"s);
        for (int i = 0; i < repetitions_count; ++i) {
            out << "London is the capital of Great Britain. "s
                << "I am travelling down the river."s
                << '\n';
        }
    }

    return 0;
}
