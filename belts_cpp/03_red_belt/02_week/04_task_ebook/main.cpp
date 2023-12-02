#include <algorithm>
#include <functional>  // std::greater
#include <iomanip>
#include <iostream>
#include <iterator>  // std::distance
#include <map>
#include <numeric>  // std::accumulate, std::partial_sum
#include <set>
#include <utility>
#include <vector>

#define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

class ReadingManagerSlow {
   public:
    ReadingManagerSlow()
        : _user_page_counts(MAX_USER_COUNT_ + 1, 0),
          _sorted_users(),
          _user_positions(MAX_USER_COUNT_ + 1, -1) {}

    void Read(int user_id, int new_page_count) {
        if (_user_page_counts[user_id] == 0) {
            AddUser(user_id);
        }

        _user_page_counts[user_id] = new_page_count;
        int& user_rank = _user_positions[user_id];

        while (true) {
            bool isRankGraterThanZero = user_rank > 0;
            bool isNeededToSwap = false;

            if (isRankGraterThanZero) {
                int prev_user_id = _sorted_users[user_rank - 1];
                isNeededToSwap = new_page_count > _user_page_counts[prev_user_id];
            }

            if (isRankGraterThanZero && isNeededToSwap) {
                SwapUsers(user_rank, user_rank - 1);
            } else {
                break;
            }
        }
    }

    double Cheer(int user_id) const {
        if (_user_page_counts[user_id] == 0) {
            return 0;
        }

        const int user_count = GetUserCount();
        if (user_count == 1) {
            return 1;
        }

        const int page_count = _user_page_counts[user_id];
        int position = _user_positions[user_id];
        while (position < user_count &&
               _user_page_counts[_sorted_users[position]] == page_count) {
            ++position;
        }

        if (position == user_count) {
            return 0;
        }

        // По умолчанию деление целочисленное, поэтому нужно привести числитель к типу double.
        // Простой способ сделать это — умножить его на 1.0.
        return (user_count - position) * 1.0 / (user_count - 1);
    }

   private:
    // Статическое поле не принадлежит какому-то конкретному объекту класса.
    // По сути это глобальная переменная, в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи следующим образом:
    // ReadingManagerSlow::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;

    std::vector<int> _user_page_counts;
    std::vector<int> _sorted_users;    // отсортированы по убыванию количества страниц
    std::vector<int> _user_positions;  // позиции в векторе _sorted_users

    int GetUserCount() const {
        return _sorted_users.size();
    }

    void AddUser(int user_id) {
        _sorted_users.push_back(user_id);
        _user_positions[user_id] = _sorted_users.size() - 1;
    }

    void SwapUsers(int lhs_position, int rhs_position) {
        const int lhs_id = _sorted_users[lhs_position];
        const int rhs_id = _sorted_users[rhs_position];
        std::swap(_sorted_users[lhs_position], _sorted_users[rhs_position]);
        std::swap(_user_positions[lhs_id], _user_positions[rhs_id]);
    }
};

class ReadingManager_V1 {
   public:
    ReadingManager_V1() = default;

    void Read(int user_id, int page_count) noexcept {
        if (!_checker.count(user_id)) {  // если пользователя с данным "user_id" ещё нет, то всё тривиально
            _checker.emplace(user_id, page_count);
            _users.emplace(user_id, page_count);
        } else {                                                                 // пользователь (чтец) с таким "user_id" уже существует
            const std::pair<int, int>& to_be_deleted = *_checker.find(user_id);  // формируем пару, которую нужно удалить из "_users"
            _checker[user_id] = page_count;                                      // актуализируем "_checker"
            _users.erase(to_be_deleted);
            _users.insert({user_id, page_count});  // наконец, актуализируем "_users"
        }
    }

    double Cheer(int user_id) const noexcept {
        if (_checker.count(user_id) == 0) {
            return 0;
        }

        const int users_count = GetUserCount();
        if (users_count == 1) {
            return 1;
        }

        // int numbers_of_lagging_users = GetNumberOfLaggingUsers(user_id);
        return static_cast<double>(GetNumberOfLaggingUsers(user_id)) / (users_count - 1);
    }

   private:
    struct comp {
        bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const {
            if (lhs.second != rhs.second) {
                return lhs.second > rhs.second;
            } else {
                return lhs.first > rhs.first;
            }
        }
    };

    int GetUserCount() const {
        return _checker.size();
    }

    int GetNumberOfLaggingUsers(int user_id) const {
        const std::pair<int, int>& p = *_checker.find(user_id);
        auto it = _users.upper_bound({0, p.second});
        return std::distance(it, _users.end());
    }

    // contains following pairs: <user_id, page_count> which are sorted by descending order of page_count
    std::set<std::pair<int, int>, comp> _users{};

    std::map<int, int> _checker{};  // user_id => page_count mapping
};

class ReadingManager_V2 {
   public:
    ReadingManager_V2() = default;

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

class ReadingManager_V3 {
   public:
    ReadingManager_V3()
        : _uid_to_pagecount(MAX_USER_COUNT_ + 1, 0),
          _rank_to_uid(),
          _uid_to_rank(MAX_USER_COUNT_ + 1, -1) {}

    void Read(int user_id, int new_page_count) {
        if (_uid_to_pagecount[user_id] == 0) {
            AddUser(user_id);
        }

        _uid_to_pagecount[user_id] = new_page_count;
        int& rank = _uid_to_rank[user_id];

        while (rank > 0 && new_page_count > _uid_to_pagecount[_rank_to_uid[rank - 1]]) {
            SwapUsersByRank(rank, rank - 1);
        }
    }

    double Cheer(int uid) const {
        if (_uid_to_pagecount[uid] == 0) {
            return 0;
        }

        const int user_count = GetUserCount();
        if (user_count == 1) {
            return 1;
        }

        return static_cast<double>(user_count - _uid_to_rank[uid] - 1) / (user_count - 1);
    }

   private:
    // Статическое поле не принадлежит какому-то конкретному объекту класса. По сути это глобальная переменная, в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи следующим образом: "ReadingManagerSlow::MAX_USER_COUNT"
    static const int MAX_USER_COUNT_ = 100'000;

    std::vector<int> _uid_to_pagecount;
    std::vector<int> _rank_to_uid;  // пользователи отсортированы по убыванию количества страниц; "rank" является индексом массива
    std::vector<int> _uid_to_rank;

    int GetUserCount() const {
        return _rank_to_uid.size();
    }

    void AddUser(int user_id) {
        _rank_to_uid.push_back(user_id);
        _uid_to_rank[user_id] = _rank_to_uid.size() - 1;
    }

    void SwapUsersByRank(int lhs_rank, int rhs_rank) {
        const int lhs_uid = _rank_to_uid[lhs_rank];
        const int rhs_uid = _rank_to_uid[rhs_rank];
        std::swap(_rank_to_uid[lhs_rank], _rank_to_uid[rhs_rank]);
        std::swap(_uid_to_rank[lhs_uid], _uid_to_rank[rhs_uid]);
    }
};

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

void RunSlowStub() {
    ReadingManagerSlow manager;

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

void RunSolution_V1() {
    ReadingManager_V1 manager;

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

void RunSolution_V2() {
    ReadingManager_V2 manager;

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

void RunSolution_V3() {
    ReadingManager_V3 manager;

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

void RunSolution_V4() {
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
            RunSolution_V4();
        }
    }

    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("Solution_V3"s);
            RunSolution_V3();
        }
    }

    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("Solution_V2"s);
            RunSolution_V2();
        }
    }

    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("Solution_V1"s);
            RunSolution_V1();
        }
    }

    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("SlowStub"s);
            RunSlowStub();
        }
    }

#else
    RunSolution_V1();
#endif  //_GLIBCXX_DEBUG
    return 0;
}