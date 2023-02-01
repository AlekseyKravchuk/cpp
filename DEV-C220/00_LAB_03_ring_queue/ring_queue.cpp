#include <iostream>
#include <string>

using namespace std::literals;

#include "../my_print.h"
#include "ring_queue.h"

int main() {
    // конструктор по списку инициализации
    RingQueue<std::string> q1{"AAA"s, "qwerty"s, "some_val"s, "another_val"s};

    bool isFirst = true;
    for (const auto& el : q1) {
        if (isFirst) {
            std::cout << el;
            isFirst = false;
        }
        std::cout << ' ' << el;
    }
    std::cout << std::endl;

    // PrintCollection(q1);

    q1.push("abc");
    PrintCollection(q1);

    q1.push("abcd");
    PrintCollection(q1);

    q1.push("abcde");
    PrintCollection(q1);

    q1.pop();
    PrintCollection(q1);

    auto q2 = q1;
    PrintCollection(q2, "q2: "s);

    return 0;
}
