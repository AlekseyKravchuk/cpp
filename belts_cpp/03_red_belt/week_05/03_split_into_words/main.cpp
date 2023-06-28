#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/version.hpp>
#include <iterator>  // std::istream_iterator
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "../../print.h"

using namespace std::literals;

std::vector<std::string> SplitIntoWords_V1(const std::string& str) {
    std::vector<std::string> result;

    std::string::const_iterator str_begin = std::begin(str);
    std::string::const_iterator str_end = std::end(str);

    while (true) {
        auto it = std::find(str_begin, str_end, ' ');

        // для сохранения текущей подстроки используем конструктор std::string от двух итераторов
        result.push_back(std::string(str_begin, it));

        if (it == str_end) {
            break;
        } else {
            while (*it == ' ') {
                ++it;
            }
            str_begin = it;
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWords_V2_string_view(const std::string& str) {
    std::string_view s_view = str;
    std::vector<std::string_view> result;

    // начинаем с нулевой позиции
    size_t pos = 0;

    // позиция "за концом" - аналог итератора "one past the end"
    const size_t pos_end = s_view.npos;

    while (true) {
        // std::string_view("что_искать", "начиная_с_какой_позиции_искать")
        size_t space_pos = s_view.find(' ', pos);

        // В результат нужно положить подстроку [pos, space_pos) текущего "string_view".
        // Метод "substr" возвращает "string_view" на подстроку.
        // начинаем подстроку с позиции "pos", а 2-ой аргумент - это длина этой подстроки,
        // также нужно учесть случай, когда пробел не нашелся и "space_pos" == "npos"
        result.push_back(space_pos == pos_end ? s_view.substr(pos, pos_end)
                                              : s_view.substr(pos, space_pos - pos));

        if (space_pos == pos_end) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {
                ++space_pos;
            }
            pos = space_pos;
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWords_V3_string_view(const std::string& str) {
    std::string_view s_view = str;
    std::vector<std::string_view> result;

    while (true) {
        // ищем первый пробел в ещё не обработанной части
        size_t space_pos = s_view.find(' ');
        
        result.push_back(s_view.substr(0, space_pos));

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {
                ++space_pos;
            }
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

std::vector<std::string> SplitIntoWords_V3(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word) {
        result.push_back(word);
    }

    return result;
}

// using std::istream_iterator
std::vector<std::string> SplitIntoWords_V4(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());

    return results;
}

// using boost library
std::vector<std::string> SplitIntoWords_V5(const std::string& str) {
    std::vector<std::string> results;
    boost::split(results, str, [](char c) { return c == ' '; });

    return results;
}

// multiple split tokens using boost::is_any_of
std::vector<std::string> SplitIntoWords_V6(const std::string& str) {
    std::vector<std::string> results;
    boost::split(results, str, boost::is_any_of(" ,"), boost::token_compress_on);

    return results;
}

int main() {
    std::cout << "Boost version: "s << BOOST_LIB_VERSION << '\n';
    std::string str = "some     words to    be      splitted into         words"s;
    // std::string s = "some, words, to, be, splitted, into, words"s;

    // boost::algorithm::trim(str);
    // std::cout << "Trimmed string: "s << str << std::endl;

    std::vector<std::string_view> v = SplitIntoWords_V3_string_view(str);

    std::cout << "Content of vector containing words: "s << v << std::endl;

    return 0;
}
