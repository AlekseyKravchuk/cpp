#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <stdexcept>
#include <algorithm>


using namespace std::literals;

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = " "s) {
    bool isFirst = true;
    std::ostringstream oss;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << delimeter;
        }
        isFirst = false;
        oss << elm;
    }
    return oss.str();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{"s << Join(s, ", "s) << "}"s;
}

int main() {
    std::string file_path = "14_input.txt";
    std::ifstream inFile(file_path);

    if (!inFile) {
        std::ostringstream oss;
        oss << "File "s << file_path << " failed to open."s;
        throw std::runtime_error(oss.str());
    }

    std::istream_iterator<std::string> inputIt(inFile);
    std::istream_iterator<std::string> endIt;

    std::set<std::string> s;

    std::copy(inputIt, endIt, std::inserter(s, s.begin()));

    // a, b, bar, c, foo, foobar
    std::cout << "Resulting unique user input: " << s << std::endl;

    return 0;
}
