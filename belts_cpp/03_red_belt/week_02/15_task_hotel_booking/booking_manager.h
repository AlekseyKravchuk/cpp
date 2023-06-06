#pragma once

#include <map>
#include <string>
#include <set>

class BookingManager_V1 {
   public:
    // client_event: hotel_name => std::set<client_identifiers> - количество уникальных uid, забронировавших номера в отеле "hotel_name"
    using client_event = std::map<std::string, std::set<uint32_t>>;

    // room_event: hotel_name => room_count - количество номеров, забронированных в отеле "hotel_name"
    using room_event = std::map<std::string, uint16_t>;

    void MakeBooking(int64_t time, std::string hotel_name, uint32_t client_id, uint16_t room_count);
    size_t GetNumberClientsBookedTimeAgo(std::string hotel_name);
    int64_t GetNumberRoomsBookedTimeAgo(std::string hotel_name);

   private:
    static const int TIME_AGO = 86400;  // in seconds
    std::map<int64_t, client_event> _time_to_client_event;
    std::map<int64_t, room_event> _time_to_room_event;

    void RemoveRedundantEvents(int64_t time);
    void AddEvent(int64_t time, std::string hotel_name, uint32_t client_id, uint16_t room_count);
};

class BookingManager_V2 {
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