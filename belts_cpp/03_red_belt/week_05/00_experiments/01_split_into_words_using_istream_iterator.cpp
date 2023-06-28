#include <algorithm>
#include <iterator>  // std::istream_iterator
#include <sstream>
#include <string>
#include <vector>

#include "../../print.h"

using namespace std::literals;


std::vector<std::string> SplitIntoWords(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    return results;
}

int main() {
    // std::string s = "          some words to be      splitted into         words           "s;
    // std::vector<std::string> v = SplitIntoWords(s);
    // std::cout << "Content of vector containing words: "s << v << std::endl;

    std::string text = "   Let   me split    this   into  words    "s;
    std::istringstream iss(text);
    std::vector v(std::istream_iterator<std::string>{iss},
                  std::istream_iterator<std::string>());
    std::cout << "Content of vector containing words: "s << v << std::endl;

    return 0;
}
