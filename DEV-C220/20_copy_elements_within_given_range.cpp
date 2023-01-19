// C помощью алгоритма "std::copy_if" ребуется скопировать только те значения,
// которые попадают в указанный диапазон

#include <algorithm>  // std::copy_if
#include <iterator>   // std::begin(), std::end(), std::back_inserter
#include <vector>

#include "my_print.h"

class RangeFunctor {
    int _lower{};
    int _upper{};

   public:
    RangeFunctor(int lower, int upper) : _lower(lower), _upper(upper) {}

    bool operator()(int val) const {
        // возвращаем "true", только если "val" попадает в заданный диапазон
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
    // std::copy_if(source.begin(), source.end(), std::back_inserter(dest), RangeFunctor(lower, upper));

    // тот же функционал, но реализованный с помощью лямбда-функции
    // в списке захвата указываем имена переменных - компилятор с такими именами в своем анонимном функторе заводит переменные
    // далее вызывает конструктор, который инициализирует эти переменные-члены переданными от пользователя значениями
    std::copy_if(source.begin(), source.end(),
                 std::back_inserter(dest),
                 [lower, upper](int val) -> bool {
                     return (val >= lower && val < upper);
                 });

    PrintCollection(dest);

    return 0;
}
