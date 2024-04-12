#pragma once

#include <string>


// класс FlightProvider выступает посредником для бронирования авиаперелетов
class FlightProvider {
   public:
    using BookingId = int;

    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date;
    };

    // Получает на вход данные о перелете и возвращает идентификатор бронирования
    // Предполагаем, что метод Book может генерировать исключение в случае, если не удалось забронировать билет на самолет
    BookingId Book(const BookingData& data);

    // Примечание: отмена бронирования никогда не приводит к сбоям
    void Cancel(BookingId id);

   private:
    BookingId _counter{0};
};

// очень похожий на FlightProvider класс для бронирования гостиниц
class HotelProvider {
   public:
    using BookingId = int;

    struct BookingData {
        std::string city;
        std::string date_from;
        std::string date_to;
    };

    // Предполагаем, что метод Book может генерировать исключение в случае, если нет доступных номеров в гостинице
    BookingId Book(const BookingData& data);

    void Cancel(BookingId id);

   private:
    BookingId _counter{0};
};

// Пытаемся использовать ранее описанные классы для разработки системы управления командировками сотрудников
// Мы хотим посылать сотрудников в командировки и нам надо знать на каких рейсах они летят в другие города и в каких гостиницах проживают.
