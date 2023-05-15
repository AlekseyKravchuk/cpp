#include <algorithm>
#include <iomanip>
#include <iterator>
#include <vector>

#include "airline_ticket.h"
#include "test_runner.h"

/*
================= Задание по программированию «Генератор компараторов» =================

Давайте представим, что вы разрабатываете инновационный сервис поиска авиабилетов AviaScanner.
В вашем сервисе авиабилет представляется в виде структуры

struct Date {
  int year, month, day;
};

struct Time {
  int hours, minutes;
};

struct AirlineTicket {
  string from;
  string to;
  string airline;
  Date departure_date;
  Time departure_time;
  Date arrival_date;
  Time arrival_time;
  uint64_t price;
};

В данный момент вы работаете над функцией сортировки результатов поиска.
Пользователь вводит свой запрос и получает список подходящих билетов.
Дальше он может задавать параметры сортировки этого списка.
Например, сначала по цене, затем по времени вылета и, наконец, по аэропорту прилёта.

Чтобы реализовать сортировку как в примере, можно воспользоваться алгоритмом цифровой сортировки:
void SortTickets(vector<AirlineTicket>& tixs) {
  stable_sort(begin(tixs), end(tixs), [](const AirlineTicket& lhs, const AirlineTicket& rhs) {
    return lhs.to < rhs.to;
  });
  stable_sort(begin(tixs), end(tixs), [](const AirlineTicket& lhs, const AirlineTicket& rhs) {
    return lhs.departure_time < rhs.departure_time;
  });
  stable_sort(begin(tixs), end(tixs), [](const AirlineTicket& lhs, const AirlineTicket& rhs) {
    return lhs.price < rhs.price;
  });
}

Как видите, в примере выше есть дублирование кода — нам пришлось написать три лямбда-функции, которые отличаются только полем,
по которому выполняется сортировка. От этого дублирования можно избавиться, написав макрос SORT_BY и применив его следующим образом:

void SortTickets(vector<AirlineTicket>& tixs) {
  stable_sort(begin(tixs), end(tixs), SORT_BY(to));
  stable_sort(begin(tixs), end(tixs), SORT_BY(departure_time));
  stable_sort(begin(tixs), end(tixs), SORT_BY(price));
}

Как видите, в примере выше есть дублирование кода — нам пришлось написать три лямбда-функции, которые отличаются только полем, по которому выполняется сортировка. От этого дублирования можно избавиться, написав макрос SORT_BY и применив его следующим образом:

void SortTickets(vector<AirlineTicket>& tixs) {
  stable_sort(begin(tixs), end(tixs), SORT_BY(to));
  stable_sort(begin(tixs), end(tixs), SORT_BY(departure_time));
  stable_sort(begin(tixs), end(tixs), SORT_BY(price));
}

Напишите макрос SORT_BY, который принимает в качестве параметра имя поля структуры AirlineTicket.
Вызов sort(begin(tixs), end(tixs), SORT_BY(some_field)) должен приводить к сортировке вектора tixs по полю some_field.

Вам дан файл airline_ticket.h, содержащий объявления структур Time, Date и AirlineTicket,
а также заготовка решения в виде cpp-файла sort_by.cpp. Пришлите на проверку cpp-файл, который:
    - подключает заголовочный файл airline_ticket.h
    - содержит макрос SORT_BY
    - содержит определения операторов, необходимых для использования классов Date и Time
      в алгоритме сортировки и макросе ASSERT_EQUAL (формат вывода в поток можете выбрать произвольный)
*/

#define SORT_BY(field)                     \
    [](const auto& lhs, const auto& rhs) { \
        return lhs.field < rhs.field;      \
    }

bool operator<(const Date& lhs, const Date& rhs) {
    return std::tuple(lhs.year, lhs.month, lhs.day) <
           std::tuple(rhs.year, rhs.month, rhs.day);
}

bool operator!=(const Date& lhs, const Date& rhs) {
    return std::tuple(lhs.year, lhs.month, lhs.day) !=
           std::tuple(rhs.year, rhs.month, rhs.day);
}

bool operator==(const Date& lhs, const Date& rhs) {
    return std::tuple(lhs.year, lhs.month, lhs.day) ==
           std::tuple(rhs.year, rhs.month, rhs.day);
}

bool operator<(const Time& lhs, const Time& rhs) {
    return std::tuple(lhs.hours, lhs.minutes) <
           std::tuple(rhs.hours, rhs.minutes);
}

bool operator!=(const Time& lhs, const Time& rhs) {
    return std::tuple(lhs.hours, lhs.minutes) !=
           std::tuple(rhs.hours, rhs.minutes);
}

bool operator==(const Time& lhs, const Time& rhs) {
    return std::tuple(lhs.hours, lhs.minutes) !=
           std::tuple(rhs.hours, rhs.minutes);
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(2) << std::setfill('0') << date.day << '-'
       << std::setw(2) << std::setfill('0') << date.month << '-'
       << std::setw(4) << std::setfill('0') << date.year;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Time& time) {
    os << std::setw(2) << std::setfill('0') << time.hours << ':'
       << std::setw(2) << std::setfill('0') << time.minutes;
    return os;
}

void TestSortBy() {
    std::vector<AirlineTicket> tixs = {
        {"VKO", "AER", "Utair", {2018, 2, 28}, {17, 40}, {2018, 2, 28}, {20, 0}, 1200},
        {"AER", "VKO", "Utair", {2018, 3, 5}, {14, 15}, {2018, 3, 5}, {16, 30}, 1700},
        {"AER", "SVO", "Aeroflot", {2018, 3, 5}, {18, 30}, {2018, 3, 5}, {20, 30}, 2300},
        {"PMI", "DME", "Iberia", {2018, 2, 8}, {23, 00}, {2018, 2, 9}, {3, 30}, 9000},
        {"CDG", "SVO", "AirFrance", {2018, 3, 1}, {13, 00}, {2018, 3, 1}, {17, 30}, 8000},
    };

    std::sort(std::begin(tixs), std::end(tixs), SORT_BY(price));
    ASSERT_EQUAL(tixs.front().price, 1200);
    ASSERT_EQUAL(tixs.back().price, 9000);

    std::sort(std::begin(tixs), std::end(tixs), SORT_BY(from));
    ASSERT_EQUAL(tixs.front().from, "AER");
    ASSERT_EQUAL(tixs.back().from, "VKO");

    std::sort(std::begin(tixs), std::end(tixs), SORT_BY(arrival_date));
    ASSERT_EQUAL(tixs.front().arrival_date, (Date{2018, 2, 9}));
    ASSERT_EQUAL(tixs.back().arrival_date, (Date{2018, 3, 5}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSortBy);
}