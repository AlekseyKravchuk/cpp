#include "booking_manager.h"

// =========================== BookingManager_V1 ===========================
void BookingManager_V1::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint16_t room_count) {
    BookingManager_V1::RemoveRedundantEvents(time);
    BookingManager_V1::AddEvent(time, hotel_name, client_id, room_count);
}

size_t BookingManager_V1::GetNumberClientsBookedTimeAgo(std::string hotel_name) {
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

int64_t BookingManager_V1::GetNumberRoomsBookedTimeAgo(std::string hotel_name) {
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

void BookingManager_V1::RemoveRedundantEvents(int64_t time) {
    if (!_time_to_client_event.empty()) {
        auto it_clients = _time_to_client_event.upper_bound(time - TIME_AGO);
        auto it_rooms = _time_to_room_event.upper_bound(time - TIME_AGO);
        if (it_clients != _time_to_client_event.begin()) {
            _time_to_client_event.erase(_time_to_client_event.begin(), it_clients);
            _time_to_room_event.erase(_time_to_room_event.begin(), it_rooms);
        }
    }
}

void BookingManager_V1::AddEvent(int64_t time,
                                 std::string hotel_name,
                                 uint32_t client_id,
                                 uint16_t room_count) {
    if (!_time_to_client_event[time][hotel_name].count(client_id)) {
        _time_to_client_event[time][hotel_name].insert(client_id);
    }

    _time_to_room_event[time][hotel_name] += room_count;
}

// =========================== BookingManager_V2 ===========================
void BookingManager_V2::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint16_t room_count) {
    RemoveRedundantEvents(time);
    AddEvent(time, hotel_name, client_id, room_count);
}

size_t BookingManager_V2::GetNumberClientsBookedTimeAgo(std::string hotel_name) {
    size_t clients_count = 0;
    for (const auto& [time, hotel_to_statistics] : _time_to_event) {
        if (hotel_to_statistics.count(hotel_name)) {
            clients_count += hotel_to_statistics.at(hotel_name).first.size();
        }
    }
    return clients_count;
}

int64_t BookingManager_V2::GetNumberRoomsBookedTimeAgo(std::string hotel_name) {
    size_t rooms_count = 0;
    for (const auto& [time, hotel_to_statistics] : _time_to_event) {
        if (hotel_to_statistics.count(hotel_name)) {
            rooms_count += hotel_to_statistics.at(hotel_name).second;
        }
    }
    return rooms_count;
}

void BookingManager_V2::RemoveRedundantEvents(int64_t time) {
    if (_time_to_event.empty()) {
        return;
    }

    auto it = _time_to_event.upper_bound(time - TIME_AGO);
    if (it == _time_to_event.begin()) {
        return;
    }

    _time_to_event.erase(_time_to_event.begin(), it);
}

void BookingManager_V2::AddEvent(int64_t time,
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
