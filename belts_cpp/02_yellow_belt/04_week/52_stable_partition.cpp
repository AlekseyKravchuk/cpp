#include <algorithm>  // std::stable_partition
#include <iostream>
#include <vector>

#include "00_print.h"

int main() {
    std::vector<int> v = {5, 2, 9, 11, 6, 19, 22, 4};
    auto it = std::stable_partition(v.begin(),
                                    v.end(),
                                    [](int value) {
                                        return value % 2 == 0;
                                        // return (value > 4) && (value < 10);
                                        // return value > 100;
                                    });
    if (it != v.begin()) {
        std::cout << "it point to element with value: "s << *it << std::endl;

        std::cout << "range for which pred returns TRUE: "s;
        PrintRange(v.begin(), it);

        std::cout << "range for which pred returns FALSE: "s;
        PrintRange(it, v.end());
    } else {
        std::cout << "pred returned FALSE for all values in vector"s << std::endl;
    }

    return 0;
}
