#include <algorithm>
#include <queue>
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

class HotelManager {
   public:
    void Book(int64_t time, const std::string& hotel_name,
              int client_id, int room_count) {
        current_time_ = time;
        hotels_[hotel_name].Book({time, client_id, room_count});
    }
    int ComputeClientCount(const std::string& hotel_name) {
        return hotels_[hotel_name].ComputeClientCount(current_time_);
    }
    int ComputeRoomCount(const std::string& hotel_name) {
        return hotels_[hotel_name].ComputeRoomCount(current_time_);
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
            last_bookings_.push(booking);
            room_count_ += booking.room_count;
            ++client_booking_counts_[booking.client_id];
        }
        int ComputeClientCount(int64_t current_time) {
            RemoveOldBookings(current_time);
            return client_booking_counts_.size();
        }
        int ComputeRoomCount(int64_t current_time) {
            RemoveOldBookings(current_time);
            return room_count_;
        }

       private:
        static const int TIME_WINDOW_SIZE = 86400;
        std::queue<Booking> last_bookings_;
        int room_count_ = 0;
        std::map<int, int> client_booking_counts_;

        void PopBooking() {
            const Booking& booking = last_bookings_.front();
            room_count_ -= booking.room_count;
            const auto client_stat_it =
                client_booking_counts_.find(booking.client_id);
            if (--client_stat_it->second == 0) {
                client_booking_counts_.erase(client_stat_it);
            }
            last_bookings_.pop();
        }
        void RemoveOldBookings(int64_t current_time) {
            while (
                !last_bookings_.empty() && last_bookings_.front().time <= current_time - TIME_WINDOW_SIZE) {
                PopBooking();
            }
        }
    };

    int64_t current_time_ = 0;
    std::map<std::string, HotelInfo> hotels_;
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