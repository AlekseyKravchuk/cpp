#include <iostream>

#include "log_duration.h"

int main() {
    // эта команда отключает какую-то синхронизацию; эту команду нужно вызывать ДО первой операции ввода-вывода
    std::ios_base::sync_with_stdio(false);

    // По умолчанию потоки "std::cout" и "std:cerr" связаны с потоком "std::cin".
    // Перед каждой операцией ввода (в "std::cin") их буферы (буферы "std::cout" и "std:cerr") сбрасываются.
    // команда "std::cin.tie(nullptr)" разрывает эту связь
    std::cin.tie(nullptr);

    {
        LOG_DURATION("endl"s);
        for (int i = 0; i < 100'000; ++i) {
            int x;
            std::cin >> x;
            std::cout << x << std::endl;
        }
    }

    {
        LOG_DURATION("'\\n'"s);
        for (int i = 0; i < 100'000; ++i) {
            int x;
            std::cin >> x;
            std::cout << x << '\n';
        }
    }

    // std::cin.tie(nullptr);
    // std::cout << "Enter two integers: "s;
    // int x, y;
    // std::cin >> x >> y;
    // for (;;) {
    // }

    return 0;
}
