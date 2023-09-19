#include <algorithm> // std::count
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>

auto my_hardware_concurrency() {
    std::ifstream cpuinfo("/proc/cpuinfo", std::ios::binary);

    return std::count(std::istream_iterator<std::string>(cpuinfo),
                      std::istream_iterator<std::string>(),
                      std::string("processor"));
}

unsigned int hardware_concurrency() {
    unsigned int cores = std::thread::hardware_concurrency();
    return cores ? cores : my_hardware_concurrency();
}

int main() {
    std::cout << "Num of cores = " << hardware_concurrency() << std::endl;

    return 0;
}
