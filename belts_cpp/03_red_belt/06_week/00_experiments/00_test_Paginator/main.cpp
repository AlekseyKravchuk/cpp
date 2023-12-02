#include <string>
#include <vector>

#include "../../../print.h"
#include "paginator.h"

int main() {
    std::vector<std::string> lines = {
        "line_1",
        "line_2",
        "line_3",
        "line_4",
        "line_5",
        "line_6",
        "line_7"};
    
    for (auto page : Paginate(lines, 2u)) {  // "page" имеет тип "IteratorRange"
        std::cout << "====================" << std::endl;
        for (auto line : page) {  // "line" имеет тип "std::string"
            std::cout << line << std::endl;
        }
    }
    return 0;
}
