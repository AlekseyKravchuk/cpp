#include <iostream>
#include <map>
#include <set>

int main() {
    auto myMap =
        std::map<std::string, int>
        {{"one", 1},
         {"two", 2},
         {"three", 3}};

    std::cout << myMap.size() << '\n';
    auto node = myMap.extract("two");
    std::cout << myMap.size() << '\n';

    return 0;
}
