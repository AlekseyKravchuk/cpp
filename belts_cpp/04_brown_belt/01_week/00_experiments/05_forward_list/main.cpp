#include <forward_list>
#include <iostream>

int main() {
    // std::forward_list<int> flist1 = {10, 20, 30};
    std::forward_list<int> flist;
    flist.push_front(22);

    std::cout << "flist contains: ";
    for (auto it = flist.begin(); it != flist.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
