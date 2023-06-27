#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>  // std::prev
#include <list>
#include <set>

#define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

class SportsmenOrdering {
   public:
    void PutInOrder(uint_fast16_t current, uint_fast16_t prev) {
        if (_checker.count(prev)) {
            auto it = std::find(_ordering.begin(), _ordering.end(), prev);
            std::advance(it, 1);
            _ordering.insert(it, current);
        } else {
            _ordering.push_back(current);
        }
    }

    void PrintInOrder() {
        // TODO: 
    }

   private:
    std::list<uint_fast16_t> _ordering{};
    std::set<uint_fast16_t> _checker{};
};

int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio,
    std::ios::sync_with_stdio(false);

    // а также выполняется отвязка std::cin от std::cout
    std::cin.tie(nullptr);

#ifdef _GLIBCXX_DEBUG
    REDIRECT_INPUT("input.txt"s);
    int n{};  // n <= 100'000
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
    }
#else
    RunSolution();
#endif  //_GLIBCXX_DEBUG
    return 0;
}