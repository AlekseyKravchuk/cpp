#include <algorithm>
#include <functional>  // std::greater
#include <iomanip>
#include <iostream>
#include <iterator>  // std::distance
#include <limits>    // std::int64_t
#include <map>
#include <numeric>  // std::accumulate, std::partial_sum
#include <set>
#include <utility>
#include <vector>

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

class BookingManager {
   public:
    // client_event: hotel_name => std::set<client_identifiers> - количество уникальных uid, забронировавших номера в отеле "hotel_name"
    using client_event = std::map<std::string, std::set<uint32_t>>;

    // room_event: hotel_name => room_count - количество номеров, забронированных в отеле "hotel_name"
    using room_event = std::map<std::string, uint16_t>;

    void MakeBooking(int64_t time,
                     std::string hotel_name,
                     uint32_t client_id,
                     uint16_t room_count) {
        RemoveRedundantEvents(time);
        AddEvent(time, hotel_name, client_id, room_count);
    }

    size_t GetNumberClientsBookedTimeAgo(std::string hotel_name) {
        std::set<uint32_t> unique_ids;
        for (const auto& [_, hname_to_ids] : _time_to_client_event) {
            for (const auto& [hname, set_of_ids] : hname_to_ids) {
                if (hname == hotel_name) {
                    unique_ids.insert(set_of_ids.begin(), set_of_ids.end());
                }
            }
        }

        return unique_ids.size();
    }

    int64_t GetNumberRoomsBookedTimeAgo(std::string hotel_name) {
        uint32_t total = 0;
        for (const auto& [_, hname_to_roomcount] : _time_to_room_event) {
            for (const auto& [hname, room_count] : hname_to_roomcount) {
                if (hname == hotel_name) {
                    total += room_count;
                }
            }
        }
        return total;
    }

   private:
    static const int TIME_AGO = 86400;  // in seconds
    std::map<int64_t, client_event> _time_to_client_event;
    std::map<int64_t, room_event> _time_to_room_event;

    void RemoveRedundantEvents(int64_t time) {
        if (!_time_to_client_event.empty()) {
            auto it_clients = _time_to_client_event.upper_bound(time - TIME_AGO);
            auto it_rooms = _time_to_room_event.upper_bound(time - TIME_AGO);
            if (it_clients != _time_to_client_event.begin()) {
                _time_to_client_event.erase(_time_to_client_event.begin(), it_clients);
                _time_to_room_event.erase(_time_to_room_event.begin(), it_rooms);
            }
        }
    }

    void AddEvent(int64_t time,
                  std::string hotel_name,
                  uint32_t client_id,
                  uint16_t room_count) {
        if (!_time_to_client_event[time][hotel_name].count(client_id)) {
            _time_to_client_event[time][hotel_name].insert(client_id);
        }

        _time_to_room_event[time][hotel_name] += room_count;
    }
};

class BookingManager_V2 {
   public:
    using hotel_to_info = std::map<std::string, std::pair<std::set<uint64_t>, uint32_t>>;
    void MakeBooking(int64_t time,
                     std::string hotel_name,
                     uint32_t client_id,
                     uint16_t room_count) {
        RemoveRedundantEvents(time);
        AddEvent(time, hotel_name, client_id, room_count);
    }

    size_t GetNumberClientsBookedTimeAgo(std::string hotel_name) {
    }

    int64_t GetNumberRoomsBookedTimeAgo(std::string hotel_name) {
    }

   private:
    static const int TIME_AGO = 86400;  // in seconds
    std::map<int64_t, hotel_to_info> _time_to_event;
    hotel_to_info _state;

    void RemoveRedundantEvents(int64_t time) {
        if (_time_to_event.empty()) {
            return;
        }

        auto it2 = _time_to_event.upper_bound(time - TIME_AGO);
        if (it2 == _time_to_event.begin()) {
            return;
        }

        hotel_to_info to_delete;
        for (auto it1 = _time_to_event.begin(); it1 != it2; std::advance(it1, 1)) {
            if (_state.empty()) {
                break;
            }

            // it1->first - это время ("time"), it1->second - это "hotel_to_info"
            const hotel_to_info& hi = it1->second;
            for (const auto& [hotel_name, p] : hi) {
                if (to_delete.count(hotel_name) == 0) {
                    to_delete.insert({hotel_name, p});
                } else {
                    to_delete[hotel_name].first.insert(p.first.begin(), p.first.end());
                    to_delete[hotel_name].second += p.second;
                }
            }
        }

        // удаляем неактуальные записи в "_state"
        for (auto it = _state.cbegin(); it != _state.cend(); /* no increment */) {
            if (must_delete) {
                it = _state.erase(it)
            } else {
                ++it;
            }
        }
    }

    void AddEvent(int64_t time,
                  std::string hotel_name,
                  uint32_t client_id,
                  uint16_t room_count) {
    }
};

void RunSolution() {
    BookingManager manager;
    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;

        if (query_type == "BOOK"s) {
            int64_t time{};
            std::cin >> time;
            std::string hotel_name{};
            std::cin >> hotel_name;
            uint32_t client_id{};
            std::cin >> client_id;
            uint16_t room_count{};
            std::cin >> room_count;
            manager.MakeBooking(time, hotel_name, client_id, room_count);
        } else if (query_type == "CLIENTS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            std::cout << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
        } else if (query_type == "ROOMS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            std::cout << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
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
#else
    RunSolution();
#endif  //_GLIBCXX_DEBUG
    return 0;
}