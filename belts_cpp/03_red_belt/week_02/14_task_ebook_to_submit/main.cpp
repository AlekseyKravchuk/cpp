#include <algorithm>
#include <functional>  // std::greater
#include <iomanip>
#include <iostream>
#include <iterator>  // std::distance
#include <map>
#include <numeric>  // std::partial_sum
#include <set>
#include <utility>
#include <vector>

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

class ReadingManager_V2_V2 {
   public:
    ReadingManager_V2_V2() = default;

    void Read(int user_id, int page_count) noexcept {
        // если пользователя с данным "user_id" ещё нет,
        // то просто добавляем данные в "_checker" и в "_users, пересчитываем partial_sums
        if (!_checker.count(user_id)) {
            _checker.emplace(user_id, page_count);
            _pcounts_to_ids[page_count].insert(user_id);
        } else {  // пользователь с данным "user_id" уже существует
            int old_page_count = _checker[user_id];
            _checker[user_id] = page_count;  // актуализируем "_checker"

            if (_pcounts_to_ids[old_page_count].size() == 1u) {
                _pcounts_to_ids.erase(old_page_count);
                _accumulated_sizes.erase(old_page_count);
            } else {
                _pcounts_to_ids[old_page_count].erase(user_id);
            }

            _pcounts_to_ids[page_count].insert(user_id);
        }

        // после каждой операции "Read" нужно обновить словарь "_accumulated_sizes"
        UpdateAccumulatedSizes();
    }

    void UpdateAccumulatedSizes() {
        int prev_sz = 0;
        for (auto& [page_count, identifiers] : _pcounts_to_ids) {
            _accumulated_sizes[page_count] = prev_sz + static_cast<int>(identifiers.size());
            prev_sz = _accumulated_sizes[page_count];
        }
    }

    double Cheer(int user_id) const noexcept {
        if (_checker.count(user_id) == 0) {
            return 0;
        }

        const int users_count = _checker.size();
        if (users_count == 1) {
            return 1;
        }

        int page_count = _checker.at(user_id);
        int numbers_of_lagging_users = users_count - _accumulated_sizes.at(page_count);
        
        return static_cast<double>(numbers_of_lagging_users) / (users_count - 1);
    }

   private:
    std::map<int, std::set<int>, std::greater<int>> _pcounts_to_ids{};
    std::map<int, int, std::greater<int>> _accumulated_sizes;
    std::map<int, int> _checker{};  // user_id => page_count mapping
};

void RunSolution() {
    ReadingManager_V2_V2 manager;

    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;
        int user_id;
        std::cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            std::cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            std::cout << std::setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }
}

int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio,
    std::ios::sync_with_stdio(false);

    // а также выполняется отвязка std::cin от std::cout
    std::cin.tie(nullptr);

#ifdef _GLIBCXX_DEBUG
    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("Solution_V2"s);
            RunSolution();
        }
    }

#else
    RunSolution();
#endif  //_GLIBCXX_DEBUG
    return 0;
}