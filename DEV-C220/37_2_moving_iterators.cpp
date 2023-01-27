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
    // теперь хочется сделать из std::set вектор с помощью перемещения
    std::set<std::string> setOfStrings = {"www"s, "abc"s, "qwerty"s};
    PrintCollection(setOfStrings, "setOfStrings befor moving: "s);

    // несмотря на использование перемещающих итераторов, будет вызвано КЛАССИЧЕСКОЕ копирование
    std::vector<std::string> vecOfStrings(std::make_move_iterator(setOfStrings.begin()),
                                          std::make_move_iterator(setOfStrings.end()));
    
    // у std::set перемещение вызвать нельзя, поскольку ключи являются КОНСТАНТНЫМИ значениями
    PrintCollection(setOfStrings, "setOfStrings AFTER moving: "s);
    PrintCollection(vecOfStrings, "vecOfStrings: "s);

    return 0;
}
