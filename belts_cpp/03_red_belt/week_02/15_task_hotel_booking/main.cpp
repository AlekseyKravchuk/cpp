#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>  // std::distance
#include <limits>    // std::int64_t
#include <numeric>   // std::accumulate, std::partial_sum
#include <utility>
#include <vector>

#include "booking_manager.h"
#include "tests.h"

#define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
#include "input_redirection.h"
#include "log_duration.h"
#endif  //_GLIBCXX_DEBUG

using namespace std::literals;

void RunSolution_V1() {
    std::cout << "==================== Solution_V1 ===================="s << std::endl;
    BookingManager_V1 manager;
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
            // std::cout << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
            std::cout << "clients at hotel \""s << hotel_name << "\": "s << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
        } else if (query_type == "ROOMS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            // std::cout << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
            std::cout << "rooms at hotel \""s << hotel_name << "\": "s << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
        }
    }
}

void RunSolution_V2() {
    std::cout << "==================== Solution_V2 ===================="s << std::endl;
    BookingManager_V2 manager;
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
            // std::cout << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
            std::cout << "clients at hotel \""s << hotel_name << "\": "s << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
        } else if (query_type == "ROOMS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            // std::cout << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
            std::cout << "rooms at hotel \""s << hotel_name << "\": "s << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
        }
    }
}

void RunSolution_V3() {
    std::cout << "==================== Solution_V3 ===================="s << std::endl;
    BookingManager_V3 manager;
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
            // std::cout << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
            std::cout << "clients at hotel \""s << hotel_name << "\": "s << manager.GetNumberClientsBookedTimeAgo(hotel_name) << "\n";
        } else if (query_type == "ROOMS"s) {
            std::string hotel_name{};
            std::cin >> hotel_name;
            // std::cout << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
            std::cout << "rooms at hotel \""s << hotel_name << "\": "s << manager.GetNumberRoomsBookedTimeAgo(hotel_name) << "\n";
        }
    }
}

int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio,
    std::ios::sync_with_stdio(false);

    // а также выполняется отвязка std::cin от std::cout
    std::cin.tie(nullptr);

#ifdef _GLIBCXX_DEBUG
    TestAll();

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
            LOG_DURATION("Solution_V2"s);
            RunSolution_V2();
        }
    }

    {
        REDIRECT_INPUT("input.txt"s);
        {
            LOG_DURATION("Solution_V3"s);
            RunSolution_V3();
        }
    }
#else
    RunSolution_V1();
    RunSolution_V2();
#endif  //_GLIBCXX_DEBUG
    return 0;
}