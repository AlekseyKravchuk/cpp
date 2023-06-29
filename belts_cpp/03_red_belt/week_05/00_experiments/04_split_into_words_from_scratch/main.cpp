#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "../../print.h"
#include "log_duration.h"

using namespace std::literals;

std::string GenerateText() {
    const int SIZE = 10'000'000;
    std::string text(SIZE, 'a');

    for (int i = 100; i < SIZE; i += 100) {
        text[i] = ' ';
    }

    return text;
}

std::vector<std::string> SplitIntoWords(const std::string& s) {
    std::vector<std::string> result;
    std::string::const_iterator it_begin = s.cbegin();
    std::string::const_iterator it_end = s.cend();

    while (true) {
        auto it = std::find(it_begin, it_end, ' ');
        result.emplace_back(it_begin, it);

        if (it == it_end) {
            break;
        } else {
            while (*it == ' ') {
                ++it;
            }
            it_begin = it;
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWordsView(std::string_view sview) {
    std::vector<std::string_view> result;

    size_t start_pos = 0;

    while (true) {
        size_t space_pos = sview.find(' ', start_pos);

        // нужно учесть случай, когда пробел не был найден => space_pos == std::string_view::npos
        result.push_back(space_pos == std::string_view::npos ? sview.substr(start_pos)
                                                             : sview.substr(start_pos, space_pos - start_pos));

        // если пробел не нейден, заканчиваем цикл
        if (space_pos == sview.npos) {
            break;
        } else {  // иначе двигаем "start_pos" на первую непробельную позицию
            while (sview[space_pos] == ' ') {
                ++space_pos;
            }
            start_pos = space_pos;
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWordsViewAdvanced(std::string_view sview) {
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = sview.find(' ');               // ищем позицию первого пробела в ещё не обработанной части
        result.emplace_back(sview.substr(0, space_pos));  // помещаем подстроку-string_view в вектор

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
    // {
    //     std::cout << "Boost lib version: "s << BOOST_LIB_VERSION << '\n';

    //     std::string str_to_be_splitted = "     String  to      be       splitted into    words     "s;
    //     std::cout << "Initial string: "s << str_to_be_splitted << std::endl;

    //     // Remove all leading and trailing white spaces from the string.
    //     boost::algorithm::trim(str_to_be_splitted);

    //     std::cout << "Trimmed string: "s << str_to_be_splitted << std::endl;

    //     std::vector<std::string> v = SplitIntoWords(str_to_be_splitted);
    //     std::cout << "Content of vector: "s << v << std::endl;
    // }

    {
        const std::string text = GenerateText();

        {
            LOG_DURATION("string"s);
            const auto words = SplitIntoWords(text);
            std::cout << words[0] << '\n';
        }

        {
            LOG_DURATION("string_view"s);
            const auto words = SplitIntoWordsView(text);
            std::cout << words[0] << '\n';
        }

        {
            LOG_DURATION("string_view_advanced"s);
            const auto words = SplitIntoWordsViewAdvanced(text);
            std::cout << words[0] << '\n';
        }
    }

    return 0;
}
