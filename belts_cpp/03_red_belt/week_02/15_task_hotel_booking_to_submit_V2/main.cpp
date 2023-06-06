#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

class BookingManager {
   public:
    // hotel_to_info: hotel_name: pair<set<client_id>, room_counter>
    using hotel_to_info = std::map<std::string, std::pair<std::set<uint32_t>, uint16_t>>;

    void MakeBooking(int64_t time, std::string hotel_name, uint32_t client_id, uint16_t room_count);
    size_t GetNumberClientsBookedTimeAgo(std::string hotel_name);
    int64_t GetNumberRoomsBookedTimeAgo(std::string hotel_name);

   private:
    static const int TIME_AGO = 86400;  // in seconds
    std::map<int64_t, hotel_to_info> _time_to_event;

    void RemoveRedundantEvents(int64_t time);
    void AddEvent(int64_t time, std::string hotel_name, uint32_t client_id, uint16_t room_count);
};

void BookingManager::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint16_t room_count) {
    RemoveRedundantEvents(time);
    AddEvent(time, hotel_name, client_id, room_count);
}

size_t BookingManager::GetNumberClientsBookedTimeAgo(std::string hotel_name) {
    size_t clients_count = 0;
    for (const auto& [time, hotel_to_statistics] : _time_to_event) {
        if (hotel_to_statistics.count(hotel_name)) {
            clients_count += hotel_to_statistics.at(hotel_name).first.size();
        }
    }
    return clients_count;
}

int64_t BookingManager::GetNumberRoomsBookedTimeAgo(std::string hotel_name) {
    size_t rooms_count = 0;
    for (const auto& [time, hotel_to_statistics] : _time_to_event) {
        if (hotel_to_statistics.count(hotel_name)) {
            rooms_count += hotel_to_statistics.at(hotel_name).second;
        }
    }
    return rooms_count;
}

void BookingManager::RemoveRedundantEvents(int64_t time) {
    if (_time_to_event.empty()) {
        return;
    }

    auto it = _time_to_event.upper_bound(time - TIME_AGO);
    if (it == _time_to_event.begin()) {
        return;
    }

    _time_to_event.erase(_time_to_event.begin(), it);
}

void BookingManager::AddEvent(int64_t time,
                                 std::string hotel_name,
                                 uint32_t client_id,
                                 uint16_t room_count) {
    if (!_time_to_event[time].count(hotel_name)) {
        _time_to_event[time][hotel_name] = std::pair{std::set{client_id}, room_count};
    } else {
        _time_to_event[time][hotel_name].first.insert(client_id);
        _time_to_event[time][hotel_name].second += room_count;
    }
}


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
            // std::cout << "clients at hotel \""s << hotel_name << "\": "s << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
        } else if (query_type == "ROOMS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            std::cout << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
            // std::cout << "rooms at hotel \""s << hotel_name << "\": "s << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
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