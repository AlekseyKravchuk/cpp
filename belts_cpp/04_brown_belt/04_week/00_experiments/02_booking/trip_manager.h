#include <string>
#include <vector>

#include "booking_managers.h"

// в этой структуре хранятся идентификаторы бронирований отелей и авиаперелетов
class Trip {
  public:
    std::vector<HotelProvider::BookingId> hotels;
    std::vector<FlightProvider::BookingId> flights;
  public:
    Trip(HotelProvider& hotel_provider, FlightProvider& flight_provider);
    ~Trip();
    Trip(const Trip&) = delete;  // запрещаем конструктор копирования
    Trip& operator=(const Trip&) = delete;

    Trip(Trip&&) = default;
    Trip& operator=(Trip&&) = default;

    void Cancel();

  private:
    // Ссылки НЕ должны быть висячими, т.е. мы неявно предполагаем, что всякий объект типа "Trip"
    // будет жить НЕ ДОЛЬШЕ, чем провайдер, переданный в него.
    // Ссылки нужно как-то проинициализировать. Единственное место, где их можно проинициализировать
    // для полей класс - это в списке инициализации конструктора.
    HotelProvider& _ref_hotels_provider;
    FlightProvider& _ref_flights_provider;
};

class TripManager {
  public:
    using BookingId = std::string;

    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    Trip Book(const BookingData& data);
    void Cancel(Trip& trip);

  private:
    HotelProvider _hotel_provider;
    FlightProvider _flight_provider;
};