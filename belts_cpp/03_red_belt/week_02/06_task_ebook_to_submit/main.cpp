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

class ReadingManager_V4 {
   public:
    ReadingManager_V4()
        : _uid_to_pcount(_MAX_USER_COUNT + 1, 0),
          _pcount_statistics(_MAX_PAGE_COUNT + 1, 0) {}

    void Read(int uid, int page_count) {
        if (_uid_to_pcount[uid] == 0) {
            _uid_to_pcount[uid] = page_count;
            ++_pcount_statistics[page_count];
            ++_user_count;
        } else {
            int old_page_count = _uid_to_pcount[uid];
            --_pcount_statistics[old_page_count];
            ++_pcount_statistics[page_count];
            _uid_to_pcount[uid] = page_count;
        }
    }

    double Cheer(int uid) const {
        if (_uid_to_pcount[uid] == 0) {
            return 0;
        }

        if (_user_count == 1u) {
            return 1;
        }

        int page_count = _uid_to_pcount[uid];
        int x = 0;
        x = std::accumulate(_pcount_statistics.begin(),
                        _pcount_statistics.begin() + page_count,
                        x);

        return static_cast<double>(x) / (_user_count - 1);
    }

   private:
    static const int _MAX_USER_COUNT = 100'000;
    static const int _MAX_PAGE_COUNT = 1'000;
    std::vector<int> _uid_to_pcount;
    std::vector<int> _pcount_statistics;
    int _user_count = 0;
};

void RunSolution() {
    ReadingManager_V4 manager;

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