#include <algorithm>
#include <iostream>
#include <numeric>  // std::accumulate
#include <string>
#include <utility>  // std::move
#include <vector>

using namespace std::literals;

template <typename Collection>
std::string JoinCollection(const Collection& c, const std::string sep = "-"s) {
    bool isFirst = true;
    return std::accumulate(c.begin(), c.end(), std::string(""s),
                           [&sep, &isFirst](std::string ret, const std::string& str) {
                               if (isFirst) {
                                   isFirst = false;
                                   return std::move(ret) + str;
                               } else {
                                   return std::move(ret) + std::move(sep) + str;
                               }
                           });
}

int main() {
    std::vector<std::string> strings = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s};
    std::string joined = JoinCollection(strings);
    std::cout << "Joined collection: "s << joined << std::endl;
    return 0;
}
