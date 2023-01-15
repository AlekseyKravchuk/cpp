#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <ostream>
#include <vector>

#include "temp.h"

using namespace std::literals;

int main() {
    std::vector<int> v_int = {1, 2, 3};
    std::copy(v_int.begin(), v_int.end(), superSeparator<int>);
    std::cout << std::endl;

    std::list<double> lst_double = {1.1, 2.2, 3.3};
    std::copy(v_int.begin(), v_int.end(), superSeparator<double>);
    std::cout << std::endl;

    std::list<std::string> lst_string = {"aba"s, "caba"s, "dada"s};
    std::copy(lst_string.begin(), lst_string.end(), superSeparator<std::string>);
    std::cout << std::endl;

    return 0;
}
