#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <utility>  // std::move

using namespace std::literals;

int main() {
    std::list<int> lst = {42};

    auto it = std::find(lst.begin(), lst.end(), 42);
    lst.insert(it, 17);

    lst.push_back(13);

    it = std::find(lst.begin(), lst.end(), 42);
    lst.insert(it, 123);

    it = std::find(lst.begin(), lst.end(), 13);
    lst.insert(it, 5);

    std::cout << "Contents of list: " << std::endl;

    for (auto it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    return 0;
}
