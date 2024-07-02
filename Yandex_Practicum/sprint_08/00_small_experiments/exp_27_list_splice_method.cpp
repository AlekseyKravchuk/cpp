#include <iostream>
#include <iterator>
#include <list>
#include <string>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    int stepSize = 3;
    // std::list<int> lst_1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    std::list<int> lst = {1, 2, 3, 4, 5};

    // итератор используемый для ликвидации элементов кратных "stepSize"
    auto it = lst.begin();

    auto nextItToBeDeleted = std::next(it, stepSize - 1);
    std::cout << "Before while loop nextItToBeDeleted points to "s << *nextItToBeDeleted << std::endl;

    std::list<int>::iterator itAfterDeleted;
    while (std::distance(nextItToBeDeleted, lst.end()) >= stepSize) {
        std::cout << "Killed item with value " << *nextItToBeDeleted
                  << "; distance to to past-the-end iterator = " << std::distance(nextItToBeDeleted, lst.end()) << std::endl;
        itAfterDeleted = lst.erase(nextItToBeDeleted);
        nextItToBeDeleted = std::next(itAfterDeleted, stepSize - 1);
    }

    std::cout << "Current state of my list: ";
    PrintRange(lst.begin(), lst.end());
    std::cout << "=========================================" << std::endl;

    lst.splice(lst.begin(), lst, itAfterDeleted, lst.end());
    std::cout << "State of my list after SPLICIING: ";
    PrintRange(lst.begin(), lst.end());
    std::cout << "=========================================" << std::endl;

    std::cout << "Now lst.begin() points to the element " << *(lst.begin()) << std::endl;

    

    return 0;
}
