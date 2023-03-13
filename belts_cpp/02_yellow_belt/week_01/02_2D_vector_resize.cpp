#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std::literals;

void PrintMatrix(std::vector<std::vector<int>> v) {
    for (const auto& row : v) {
        bool isFirstInRow = true;
        for (const auto& elm : row) {
            if (isFirstInRow) {
                std::cout << std::setw(2) << std::left << elm;
                isFirstInRow = false;
            } else {
                std::cout << ' ' << std::setw(2) << std::left << elm;
            }
        }
        std::cout << std::endl
                  << std::flush;
    }
}

int main() {
    std::vector<std::vector<int>> v{
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15}};

    v.assign(2, std::vector<int>(4, 0));

    PrintMatrix(v);

    return 0;
}
