#include "booking_manager.h"

// =========================== BookingManager_V1 ===========================
void BookingManager_V1::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint32_t room_count) {
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
                                 uint32_t room_count) {
    if (!_time_to_client_event[time][hotel_name].count(client_id)) {
        _time_to_client_event[time][hotel_name].insert(client_id);
    }

    _time_to_room_event[time][hotel_name] += room_count;
}

// =========================== BookingManager_V2 ===========================
void BookingManager_V2::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint32_t room_count) {
    RemoveRedundantEvents(time);
    AddEvent(time, hotel_name, client_id, room_count);
}

size_t BookingManager_V2::GetNumberClientsBookedTimeAgo(std::string hotel_name) {
    std::set<uint32_t> unique_client_ids;
    for (const auto& [time, hotel_to_statistics] : _time_to_event) {
        if (hotel_to_statistics.count(hotel_name)) {
            unique_client_ids.insert(hotel_to_statistics.at(hotel_name).first.begin(), hotel_to_statistics.at(hotel_name).first.end());
        }
    }
    return unique_client_ids.size();
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
                                 uint32_t room_count) {
    if (!_time_to_event[time].count(hotel_name)) {
        _time_to_event[time][hotel_name] = std::pair{std::set{client_id}, room_count};
    } else {
        _time_to_event[time][hotel_name].first.insert(client_id);
        _time_to_event[time][hotel_name].second += room_count;
    }
}

// =========================== BookingManager_V3 ===========================
void BookingManager_V3::MakeBooking(int64_t time,
                                    std::string hotel_name,
                                    uint32_t client_id,
                                    uint32_t room_count) {
    AddEvent(time, hotel_name, client_id, room_count);
    UpdateHotelStatistics(time);
}

size_t BookingManager_V3::GetNumberClientsBookedTimeAgo(std::string hotel_name) const {
    return _hotel_statistics.count(hotel_name) ? _hotel_statistics.at(hotel_name).first.size()
                                               : 0;
}

int64_t BookingManager_V3::GetNumberRoomsBookedTimeAgo(std::string hotel_name) const {
    return _hotel_statistics.count(hotel_name) ? _hotel_statistics.at(hotel_name).second
                                               : 0;
}

void BookingManager_V3::UpdateHotelStatistics(int64_t time) {
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

void BookingManager_V3::AddEvent(int64_t time,
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
