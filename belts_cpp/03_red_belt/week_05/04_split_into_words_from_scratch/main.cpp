#include <boost/version.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "../../print.h"

using namespace std::literals;

std::string GenerateText() {
    const int SIZE = 10'000'000;
    std::string text(SIZE, 'a');

    for (int i = 100; i < SIZE; i += 100) {
        text[i] = ' ';
    }

    return text;
}

std::vector<std::string_view> SplitIntoWords(const std::string& s) {
    std::string_view s_view = s;
    std::vector<std::string_view> result;

    size_t start_pos = 0;

    while (true) {
        size_t space_pos = s_view.find(' ', start_pos);

        // нужно учесть случай, когда пробел не был найден => space_pos == std::string_view::npos
        result.push_back(space_pos == std::string_view::npos ? s_view.substr(start_pos)
                                                             : s_view.substr(start_pos, space_pos - start_pos));
        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {
                ++space_pos;
            }
            start_pos = space_pos;
        }
    }

    return result;
}

int main() {
    std::cout << "Boost lib version: "s << BOOST_LIB_VERSION << '\n';
    std::string str_to_be_splitted = "String  to      be       splitted into    words"s;
    std::vector<std::string_view> v_sview = SplitIntoWords(str_to_be_splitted);

    std::cout << "Contens of vector containing string views: "s << v_sview << std::endl;

    return 0;
}
