#include <iostream>
#include <iterator>  // std::move_iterator, std::make_move_iterator
#include <ostream>
#include <set>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    std::vector<std::string> vecOfStrings = {"www"s, "abc"s, "qwerty"s};
    using IT = std::vector<std::string>::iterator;
    PrintCollection(vecOfStrings, "vecOfStrings: "s);

    // std::set<std::string> setOfStrings(std::move_iterator<IT>(vecOfStrings.begin()),
    //                                    std::move_iterator<IT>(vecOfStrings.end()));

    std::set<std::string> setOfStrings(std::make_move_iterator(vecOfStrings.begin()),
                                       std::make_move_iterator(vecOfStrings.end()));

    PrintCollection(setOfStrings, "setOfStrings: "s);

    // после перемещения всех элементов из вектора, в нем останутся пустые строки и, соответственно, size() и capacity() останутся теми же
    // это обусловлено тем, что перемещающий конструктор копирования для строк реализован таким способом
    PrintCollection(vecOfStrings, "vecOfStrings after moving elements to set: "s);
    std::cout << "vecOfStrings.size() = "s  << vecOfStrings.size() << ", vecOfStrings.capacity() = " << vecOfStrings.capacity() << std::endl;

    vecOfStrings.clear();
    std::cout << "vecOfStrings.size() = "s  << vecOfStrings.size() << ", vecOfStrings.capacity() = " << vecOfStrings.capacity() << std::endl;

    vecOfStrings.shrink_to_fit();
    std::cout << "vecOfStrings.size() = "s  << vecOfStrings.size() << ", vecOfStrings.capacity() = " << vecOfStrings.capacity() << std::endl;

    return 0;
}
