#include <iostream>
#include <map>
#include <utility>

typedef std::pair<std::string, std::string> pair;

struct comp {
    template <typename T>
    bool operator()(const T &l, const T &r) const {
        if (l.first == r.first) {
            return l.second > r.second;
        }

        return l.first < r.first;
    }
};

int main() {
    // std::map<pair, int, comp> map = {
    //     {std::make_pair("C++", "C++14"), 2014},
    //     {std::make_pair("C++", "C++17"), 2017},
    //     {std::make_pair("Java", "Java 7"), 2011},
    //     {std::make_pair("Java", "Java 8"), 2014},
    //     {std::make_pair("C", "C11"), 2011}};

    std::map<pair, int, comp> map = {
        {{"C++", "C++14"}, 2014},
        {{"C++", "C++17"}, 2017},
        {{"Java", "Java 7"}, 2011},
        {{"Java", "Java 8"}, 2014},
        {{"C", "C11"}, 2011}};

    for (const auto &[key, value] : map) {
        std::cout << "{" << key.first << "," << key.second << "}, "
                  << value << std::endl;
    }

    return 0;
}