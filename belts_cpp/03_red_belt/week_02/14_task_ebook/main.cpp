#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>  // std::distance
#include <map>
#include <set>
#include <utility>
#include <vector>

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

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

    void Read(int user_id, int page_count) {
        if (_user_page_counts[user_id] == 0) {
            AddUser(user_id);
        }

        _user_page_counts[user_id] = page_count;
        int& position = _user_positions[user_id];
        while (position > 0 && page_count > _user_page_counts[_sorted_users[position - 1]]) {
            SwapUsers(position, position - 1);
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

class ReadingManager {
   public:
    ReadingManager() = default;

    void Read(int user_id, int page_count) {
        if (!_checker.count(user_id)) {  // если пользователя с данным "user_id" ещё нет, то всё тривиально
            _checker.emplace(user_id, page_count);
            _users.emplace(user_id, page_count);
        } else {                                                          // пользователь (чтец) с таким "user_id" уже существует
            std::pair<int, int> to_be_deleted = *_checker.find(user_id);  // формируем пару, которую нужно удалить из "_users"
            _checker.at(user_id) = page_count;                            // актуализируем "_checker"
            _users.erase(to_be_deleted);
            _users.insert({user_id, page_count});  // наконец, актуализируем "_users"
        }
    }

    double Cheer(int user_id) const {
        if (_checker.count(user_id) == 0) {
            return 0;
        }

        const int users_count = GetUserCount();
        if (users_count == 1) {
            return 1;
        }

        int numbers_of_lagging_users = GetNumberOfLaggingUsers(user_id);
        return static_cast<double>(numbers_of_lagging_users) / (users_count - 1);
    }

   private:
    struct comp {
        bool operator()(std::pair<int, int> lhs, std::pair<int, int> rhs) const {
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
        std::pair<int, int> p = *_checker.find(user_id);
        auto it = _users.upper_bound({0, p.second});
        return std::distance(it, _users.end());
    }

    // contains following pairs: <user_id, page_count> which are sorted by descending order of page_count
    std::set<std::pair<int, int>, comp> _users{};

    std::map<int, int> _checker{};  // user_id => page_count mapping
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
            LOG_DURATION("Solution"s);
            RunSolution();
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
    RunSolution();
#endif  //_GLIBCXX_DEBUG
    return 0;
}