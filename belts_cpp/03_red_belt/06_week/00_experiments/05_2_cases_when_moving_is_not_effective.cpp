#include <array>
#include <iostream>
#include <utility>
#include <vector>

#include "../../log_duration.h"

std::string MakeString() {
    return "C++";
}

int main() {
    std::vector<std::string> strings;

    // !!! Забрать данные у константного объекта невозможно !!!
    const std::string s = MakeString();  // константная переменная не может отдать свои данные другому объекту

    std::cout << s << "\n";
    strings.push_back(std::move(s));  // перемещения не произошло, поскольку перемещения для константной переменной НЕ работает!!!
    std::cout << s << "\n";

    return 0;
}
