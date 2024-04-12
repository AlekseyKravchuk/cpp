#include "booking_managers.h"

#include <stdexcept>
#include <iostream>

using namespace std;

FlightProvider::BookingId FlightProvider::Book(const BookingData& data) {
    // просто увеличиваем счетчик активных бронирований
    ++_counter;

    // Имитируем ситуацию отсутствия свободных билетов:
    if (_counter > 1) {
        throw std::runtime_error("No airline tickets available");
    }
    cerr << "Flight booking: " << _counter << "\n";

    return _counter;
}

// Примечание: отмена бронирования никогда не приводит к сбоям
void FlightProvider::Cancel(BookingId id) {
    --_counter;
    cerr << "Cancel flight: " << id << "\n";
}

// =================================================================

HotelProvider::BookingId HotelProvider::Book(const BookingData& data) {
    ++_counter;
    cerr << "Hotel booking: " << _counter << "\n";
    return _counter;
}

void HotelProvider::Cancel(BookingId id) {
    --_counter;
    cerr << "Cancel hotel: " << id << "\n";
}
