#include "ring_queue.h"

#include <iostream>
#include <string>

#include "../my_print.h"

using namespace std::literals;

int main() {
    RingQueue<std::string> q1 = {"AAA"s, "BBB"s, "CCC"s, "DDD"s};
    // RingQueue<std::string> q1 = {"1"s, "2"s, "3"s, "4"s};

    // for (const auto& elm : q1) {
    //     std::cout << elm << " ";
    // }
    // std::cout << std::endl;

    PrintCollection(q1);

    // std::cout << *q1.begin() << std::endl;
    // std::cout << *q1.end() << std::endl;

    return 0;
}
