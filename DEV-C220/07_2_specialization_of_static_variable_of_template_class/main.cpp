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
    // создаем итератор, который умеет выводить целые числа в std::ostream
    std::ostream_iterator<int> it_os_int(std::cout, separator<int>);
    std::copy(v_int.begin(), v_int.end(), it_os_int);
    std::cout << std::endl;

    std::list<double> lst_double = {1.1, 2.2, 3.3};
    std::ostream_iterator<double> it_os_double(std::cout, separator<double>);
    std::copy(v_int.begin(), v_int.end(), it_os_double);
    std::cout << std::endl;

    std::list<std::string> lst_string = {"aba"s, "caba"s, "dada"s};
    std::ostream_iterator<std::string> it_os_string(std::cout, separator<std::string>);
    std::copy(lst_string.begin(), lst_string.end(), it_os_string);
    std::cout << std::endl;

    return 0;
}
