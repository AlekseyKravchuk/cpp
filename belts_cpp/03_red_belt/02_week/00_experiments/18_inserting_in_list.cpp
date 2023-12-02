#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <utility>  // std::move

using namespace std::literals;

int main() {
    std::list<int> lst;
    lst.push_back(42);
    auto it = std::find(lst.begin(), lst.end(), 42);
    lst.insert(it, 17);
    lst.push_back(13);
    it = std::find(lst.begin(), lst.end(), 42);
    lst.insert(it, 123);
    it = std::find(lst.begin(), lst.end(), 13);
    lst.insert(it, 5);

    // std::list<int> lst_2 = {11, 12, 13, 14, 15, 16, 17};

    // требуется вставить элемент "88" после "5"
    // lst.insert(lst.end(), 88);

    std::cout << "Contents of list l1 after inserting operation" << std::endl;

    for (auto it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    return 0;
}
