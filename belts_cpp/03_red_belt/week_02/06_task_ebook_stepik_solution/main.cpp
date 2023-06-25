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

class ReadingManager {
   public:
    ReadingManager()
        // -1 значит, что не случилось ни одного READ
        : _user_page_counts(_MAX_USER_COUNT + 1, -1),
          _page_achieved_by_count(_MAX_PAGE_COUNT + 1, 0) {}

    void Read(int user_id, int page_count) {
        UpdatePageRange(_user_page_counts[user_id] + 1, page_count + 1);
        _user_page_counts[user_id] = page_count;
    }

    double Cheer(int user_id) const {
        const int pages_count = _user_page_counts[user_id];
        if (pages_count == -1) {
            return 0;
        }
        const int user_count = GetUserCount();
        if (user_count == 1) {
            return 1;
        }
        // По умолчанию деление целочисленное, поэтому
        // нужно привести числитель к типу double.
        // Простой способ сделать это — умножить его на 1.0.
        return (user_count - _page_achieved_by_count[pages_count]) * 1.0 / (user_count - 1);
    }

   private:
    // Статическое поле не принадлежит какому-либо конкретному объекту класса.
    // По сути это глобальная переменная, в данном случае - константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int _MAX_USER_COUNT = 100'000;
    static const int _MAX_PAGE_COUNT = 1'000;

    // Номер страницы, до которой дочитал пользователь <ключ>
    std::vector<int> _user_page_counts;

    // Количество пользователей, дочитавших (как минимум) до страницы <индекс>
    std::vector<int> _page_achieved_by_count;

    int GetUserCount() const {
        return _page_achieved_by_count[0];
    }

    // lhs включительно, rhs не включительно
    void UpdatePageRange(int lhs, int rhs) {
        for (int i = lhs; i < rhs; ++i) {
            ++_page_achieved_by_count[i];
        }
    }
};

void RunSolution() {
    ReadingManager manager;

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
            LOG_DURATION("Solution_V4"s);
            RunSolution();
        }
    }

#else
    RunSolution();
#endif  //_GLIBCXX_DEBUG
    return 0;
}