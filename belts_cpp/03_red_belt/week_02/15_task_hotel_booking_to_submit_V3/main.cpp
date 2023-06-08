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
    // hotel_to_info: hotel_name: pair<map<client_id, room_count>, hotel_total_room_count>>
    using hotel_to_info = std::map<std::string, std::pair<std::map<uint32_t, uint32_t>, uint32_t>>;

    void MakeBooking(int64_t time, std::string hotel_name, uint32_t client_id, uint32_t room_count);
    size_t GetNumberClientsBookedTimeAgo(std::string hotel_name) const;
    int64_t GetNumberRoomsBookedTimeAgo(std::string hotel_name) const;

   private:
    static const int TIME_AGO = 86400;  // in seconds
    std::map<int64_t, hotel_to_info> _time_to_event;
    hotel_to_info _hotel_statistics;

    void UpdateHotelStatistics(int64_t time);
    void AddEvent(int64_t time, std::string hotel_name, uint32_t client_id, uint32_t room_count);
};

void BookingManager::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint32_t room_count) {
    AddEvent(time, hotel_name, client_id, room_count);
    UpdateHotelStatistics(time);
}

size_t BookingManager::GetNumberClientsBookedTimeAgo(std::string hotel_name) const {
    return _hotel_statistics.count(hotel_name) ? _hotel_statistics.at(hotel_name).first.size()
                                               : 0;
}

int64_t BookingManager::GetNumberRoomsBookedTimeAgo(std::string hotel_name) const {
    return _hotel_statistics.count(hotel_name) ? _hotel_statistics.at(hotel_name).second
                                               : 0;
}

void BookingManager::UpdateHotelStatistics(int64_t time) {
    if (_time_to_event.empty()) {
        return;
    }

    auto it_start_saved = _time_to_event.upper_bound(time - TIME_AGO);
    if (it_start_saved == _time_to_event.begin()) {
        return;
    }

    // удаляем из сводной статистики "_hotel_statistics" ненужные записи
    // и удаляем записи от "_time_to_event.begin()" до "it_start_saved" в "_time_to_event"
    for (auto it = _time_to_event.begin(); it != it_start_saved;) {
        for (const auto& [hotel_name, p] : it->second) {
            // "id_to_roomcount" is map<client_id, room_count> (of type: "std::map<uint32_t, uint32_t>")
            for (auto& [client_id, room_count] : p.first) {
                _hotel_statistics[hotel_name].first[client_id] -= room_count;
                _hotel_statistics[hotel_name].second -= room_count;

                if (_hotel_statistics[hotel_name].first[client_id] == 0) {
                    _hotel_statistics[hotel_name].first.erase(client_id);
                }

                if (_hotel_statistics[hotel_name].second == 0) {
                    _hotel_statistics.erase(hotel_name);
                }
            }
        }
        it = _time_to_event.erase(it);
    }
}

void BookingManager::AddEvent(int64_t time,
                                 std::string hotel_name,
                                 uint32_t client_id,
                                 uint32_t room_count) {
    // добавляем запись в "_time_to_event"
    if (!_time_to_event[time].count(hotel_name)) {
        _time_to_event[time][hotel_name].first.insert({client_id, room_count});
        _time_to_event[time][hotel_name].second = room_count;
    } else {
        _time_to_event[time][hotel_name].first[client_id] += room_count;

        // увеличиваем количество всех забронированных номеров в отеле
        _time_to_event[time][hotel_name].second += room_count;
    }

    // по сути те же действия выполняем для сводной статистики "_hotel_statistics"
    if (!_hotel_statistics.count(hotel_name)) {
        _hotel_statistics[hotel_name].first.insert({client_id, room_count});
        _hotel_statistics[hotel_name].second = room_count;
    } else {
        _hotel_statistics[hotel_name].first[client_id] += room_count;

        // увеличиваем количество всех забронированных номеров в отеле
        _hotel_statistics[hotel_name].second += room_count;
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
            uint32_t room_count{};
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