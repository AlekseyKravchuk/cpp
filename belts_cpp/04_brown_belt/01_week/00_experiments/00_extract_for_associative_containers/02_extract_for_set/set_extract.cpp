#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility> // std::pair

#include "print.h"

int main() {
    std::set<std::pair<int, std::string>> s = {
        {1, "milk"},
        {5, "cow"},
        {2, "girl"},
        {4, "moon"},
        {3, "brother"}};

    std::cout << "s.size() = " << s.size() << std::endl; // 5

    // extract and check if node handle is not empty before using it
    if (auto nh = s.extract({2, "girl"}); !nh.empty()) {
        nh.value() = std::pair<int, std::string>{22, "boy"};
        s.insert(std::move(nh));
    }

    for (const auto& [num, str] : s) {
         std::cout << "{" << num << " => " << str << "}" << std::endl;
    }
   

    return 0;
}
