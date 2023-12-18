#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility> // std::pair, std::move
#include <vector>

#include "print.h"

std::pair<std::string_view, std::string_view> Split(std::string_view sview, char delim) {
    size_t pos = sview.find(delim);
    return {sview.substr(0, pos), sview.substr(pos+1)};
}

int main() {
    std::string fname = "input.txt";
    std::fstream input(fname);

    if (!input) {
        std::cerr << "File \"" << fname << "\" is not opened. Exiting." << std::endl;
    }

    std::vector<std::pair<std::string, std::string>> result;
    std::string category{}, value{};
    for (std::string line; std::getline(input, line) && line != "\n";) {
        auto [category, value] = Split(line, '=');
        result.emplace_back(std::string(category), std::string(value));
    }

    std::cout << result << std::endl;

    return 0;
}
