#include <array>
#include <iostream>
#include <vector>

#include "../../log_duration.h"

const int SIZE = 10'000;

std::array<int, SIZE> MakeArray() {
    std::array<int, SIZE> a;
    a.fill(8);

    return a;
}

/*
!!! Если у объекта много данных на стеке, то перемещение НЕ поможет или поможет слишком незначительно !!!
*/

// продемонстрируем на примере массива (std::array), который хранит свои данные на стеке (у него нет данных в куче),
// что перемещение не влияет на производительность при работе с ним
int main() {
    {
        LOG_DURATION("with temp variable");
        std::vector<std::array<int, SIZE>> arrays;

        for (int i = 0; i < 10'000; ++i) {
            auto heavy_array = MakeArray();
            arrays.push_back(heavy_array);
        }
    }

    {
        LOG_DURATION("WITHOUT variable");
        std::vector<std::array<int, SIZE>> arrays;
        for (int i = 0; i < 10'000; ++i) {
            arrays.push_back(MakeArray());
        }
    }
    return 0;
}
