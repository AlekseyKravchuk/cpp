#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

/*
================================ Оценка сложности ================================
Сложность O(QL*logQ):
Исходя из кода в main, можно заметить, что общая сложность определяется как суммарная сложность
всех вызовов методов Book, ComputeClientCount и ComputeRoomCount класса HotelManager, которые определяются значением ключа query_type.
Оценим суммарные сложности всех потенциальных вызовов каждого из указанных методов, опуская несущественные, с точки зрения асимптотики,
участки кода.

Book: Поиск названия отеля по словарю hotels_ : O(L logQ). Полагаем, что отелей не могло быть создано больше, чем было запросов.
Запросов по условию не более Q. O(L) присутствует в оценке, так как для поиска в map'е мы сравниваем между собой строковые ключи,
длина которых не превосходит L.

Вызов метода Book класса HotelInfo. Поиск клиента в словаре client_booking_counts_ займет не более O(logQ).
Клиентов, вообще говоря, также не могло быть создано больше, чем было запросов.
Итого, суммарная оценка одного вызова метода Book класса HotelManager: O(L logQ) + O(logQ) = O(L logQ) путём поглощения.
Всего вызовов метода Book класса HotelManager не могло быть более Q: таким образом, итоговая оценка сложности всех потенциальных
вызовов указанного метода Book: O(QL logQ).

ComputeClientCount, ComputeRoomCount: Сложность работы данных методов, вообще говоря, определяется в обоих случаях,
как и в случае с методом Book, суммой сложности поиска названия отеля по словарю hotels_ и сложности метода RemoveOldBookings.

Сложность RemoveOldBookings, в свою очередь, зависит от сложности метода PopBooking.
При этом, несмотря на цикл, суммарно метод RemoveOldBookings вызовет метод PopBooking за время работы приложения не более Q раз,
так как каждое добавленное событие может быть удалено не более одного раза. Сложность метода PopBooking зависит от поиска и
удаления в словаре client_booking_counts_. Поиск оценивается, как O(logQ), удаление — аналогично. 
Суммарное время работы PopBooking: O(logQ) + O(logQ) = O(logQ) путём поглощения.
Таким образом, итоговая оценка сложности всех потенциальных вызовов метода RemoveOldBooking: O(Q logQ),
а методов ComputeClientCount, ComputeRoomCount: O(QL logQ) + O(Q logQ) = O(QL log Q) путём поглощения.

Итого, путём поглощения получаем итоговую оценку O(QL logQ).
Величины T, C и W в оценке сложности не участвуют: T и C определяют лишь необходимую размерность типов,
а W влияет лишь на количество удалений: чем больше W, тем меньше в конечном счёте будет вызовов методов PopBooking.
*/

using namespace std::literals;

class HotelManager {
   public:
    void Book(int64_t time,
              const std::string& hotel_name,
              int client_id,
              int room_count) {
        _current_time = time;
        _hotels[hotel_name].Book({time, client_id, room_count});
    }

    int ComputeClientCount(const std::string& hotel_name) {
        return _hotels[hotel_name].ComputeClientCount(_current_time);
    }

    int ComputeRoomCount(const std::string& hotel_name) {
        return _hotels[hotel_name].ComputeRoomCount(_current_time);
    }

   private:
    struct Booking {
        int64_t time;
        int client_id;
        int room_count;
    };

    class HotelInfo {
       public:
        void Book(const Booking& booking) {
            _last_bookings.push(booking);
            _room_count += booking.room_count;
            ++_client_booking_counts[booking.client_id];
        }

        int ComputeClientCount(int64_t current_time) {
            RemoveOldBookings(current_time);
            return _client_booking_counts.size();
        }

        int ComputeRoomCount(int64_t current_time) {
            RemoveOldBookings(current_time);
            return _room_count;
        }

       private:
        static const int TIME_WINDOW_SIZE = 86400;
        std::queue<Booking> _last_bookings;
        int _room_count = 0;
        std::map<int, int> _client_booking_counts;

        void PopBooking() {
            const Booking& booking = _last_bookings.front();
            _room_count -= booking.room_count;
            const auto client_stat_it =
                _client_booking_counts.find(booking.client_id);
            if (--client_stat_it->second == 0) {
                _client_booking_counts.erase(client_stat_it);
            }
            _last_bookings.pop();
        }
        void RemoveOldBookings(int64_t current_time) {
            while (
                !_last_bookings.empty() && _last_bookings.front().time <= current_time - TIME_WINDOW_SIZE) {
                PopBooking();
            }
        }
    };

    int64_t _current_time = 0;
    std::map<std::string, HotelInfo> _hotels;
};

void RunSolution() {
    HotelManager manager;
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
            manager.Book(time, hotel_name, client_id, room_count);
        } else if (query_type == "CLIENTS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            std::cout << manager.ComputeClientCount(hotel_name) << "\n";
            // std::cout << "clients at hotel \""s << hotel_name << "\": "s << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
        } else if (query_type == "ROOMS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            std::cout << manager.ComputeRoomCount(hotel_name) << "\n";
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