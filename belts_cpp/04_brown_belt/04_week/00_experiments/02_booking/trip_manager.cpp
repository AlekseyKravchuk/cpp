#include "trip_manager.h"

Trip TripManager::Book(const BookingData& data) {
    Trip trip(_hotel_provider, _flight_provider);

    // !!! Если в какой-то момент одно из бронирований окажется неудачным и произойдет исключение,
    // то раскрутка стека гарантирует нам, что ВСЕ созданные к этому моменту автоматические объекты
    // будут уничтожены - для всех них будет вызван соответствующий деструктор

    // В простейшем случае командировка - это:
    // 1) перелет ТУДА;
    // 2) проживание в гостинице;
    // 3) перелет ОБРАТНО.

    // 1) перелет ТУДА
    {
        FlightProvider::BookingData data;
        trip.flights.push_back(_flight_provider.Book(data));
    }

    // 2) проживание в гостинице
    {
        HotelProvider::BookingData data;
        trip.hotels.push_back(_hotel_provider.Book(data));
    }

    // 3) перелет ОБРАТНО
    {
        FlightProvider::BookingData data;
        trip.flights.push_back(_flight_provider.Book(data));
    }


    return trip;
}

void TripManager::Cancel(Trip& trip) {
//    for (auto& id: trip.hotels) {
//        _hotel_provider.Cancel(id);
//    }
//    trip.hotels.clear();
//
//    for (auto& id: trip.flights) {
//        _flight_provider.Cancel(id);
//    }
//    trip.flights.clear();
    trip.Cancel();
}

Trip::Trip(HotelProvider& hotel_provider,
           FlightProvider& flight_provider)
        : _ref_hotels_provider(hotel_provider),
          _ref_flights_provider(flight_provider) {}

Trip::~Trip() {
    Cancel();
}

void Trip::Cancel() {
    for (auto& id: hotels) {
        _ref_hotels_provider.Cancel(id);
    }
    hotels.clear();

    for (auto& id: flights) {
        _ref_flights_provider.Cancel(id);
    }
    flights.clear();
}