// Требуется скопировать только те значения, которые попадают в указанный диапазон

#include <algorithm>  // std::copy_if
#include <iterator>   // std::begin(), std::end(), std::back_inserter
#include <vector>

#include "my_print.h"

class Range {
    int _lower{};
    int _upper{};

   public:
    Range(int lower, int upper) : _lower(lower), _upper(upper) {}

    bool operator()(int val) const {
        return (val >= _lower && val < _upper);
    }
};

int main() {
    // std::vector<int> source = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> source = {20, 18, 11, 3, 7, 22, 45, };
    std::vector<int> dest;

    int lower = 2;
    int upper = 23;

    // шаблонная функция "std::back_inserter" формирует итератор вставки "std::back_insert_iterator"
    // std::copy_if(source.begin(), source.end(), std::back_inserter(dest), Range(lower, upper));

    // тот же функционал, но реализованный с помощью лямбда-функции
    std::copy_if(source.begin(), source.end(), std::back_inserter(dest),
                 [lower, upper](int val) -> bool {
                     return (val >= lower && val < upper);
                 });

    PrintCollection(dest);

    return 0;
}
