#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "../../print.h"

using namespace std::literals;

std::vector<std::string_view> SplitIntoWordsView(const std::string& text) {
    std::vector<std::string_view> result;
    std::string_view sview = text;

    while (true) {
        size_t space_pos = sview.find(' ');            // ищем позицию первого пробела в "string_view"
        result.push_back(sview.substr(0, space_pos));  // помещаем подстроку "string_view" в вектор

        if (space_pos == sview.npos) {
            break;
        } else {
            while (sview[space_pos] == ' ') {
                ++space_pos;
            }
            sview.remove_prefix(space_pos);
        }
    }

    return result;
}

int main() {
    std::string str = "AB         CD EF     GHI"s;
    std::vector<std::string_view> v = SplitIntoWordsView(str);
    std::cout << "Content of vector of words: "s << v << std::endl;

    // std::string_view data("ABCDEF"s);
    // std::cout << data.substr(0, data.npos) << std::endl;
    // std::cout << data.substr(0, 1) << std::endl;
    // std::cout << data.substr(0, 2) << std::endl;

}