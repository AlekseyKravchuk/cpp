#include <algorithm>
#include <iomanip>
#include <iterator>
#include <map>
#include <vector>

#include "airline_ticket.h"
#include "test_runner.h"

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

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(4) << std::setfill('0') << date.year << '-'
       << std::setw(2) << std::setfill('0') << date.month << '-'
       << std::setw(2) << std::setfill('0') << date.day;
    return os;
}

std::istream& operator>>(std::istream& is, Date& date) {
    is >> date.year;
    is.ignore(1);
    is >> date.month;
    is.ignore(1);
    is >> date.day;
    return is;
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

std::ostream& operator<<(std::ostream& os, const Time& time) {
    os << std::setw(2) << std::setfill('0') << time.hours << ':'
       << std::setw(2) << std::setfill('0') << time.minutes;
    return os;
}

std::istream& operator>>(std::istream& is, Time& time) {
    is >> time.hours;
    is.ignore(1);
    is >> time.minutes;
    return is;
}

std::map<std::string, std::string>::const_iterator it;

#define UPDATE_FIELD(ticket, field, values) \
    it = values.find(#field);               \
    if (it != values.end()) {               \
        std::istringstream iss(it->second); \
        iss >> ticket.field;                \
    }

void UpdateTicket(AirlineTicket& ticket,
                  const std::map<std::string, std::string>& updates) {
    UPDATE_FIELD(ticket, from, updates);
    UPDATE_FIELD(ticket, to, updates);
    UPDATE_FIELD(ticket, airline, updates);
    UPDATE_FIELD(ticket, departure_date, updates);
    UPDATE_FIELD(ticket, departure_time, updates);
    UPDATE_FIELD(ticket, arrival_date, updates);
    UPDATE_FIELD(ticket, arrival_time, updates);
    UPDATE_FIELD(ticket, price, updates);
}

void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    // #1 ИЗМЕНЕНИЯ
    const std::map<std::string, std::string> updates1 = {
        {"departure_date", "2018-2-28"},
        {"departure_time", "17:40"},
    };

    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    // #2 ИЗМЕНЕНИЯ
    const std::map<std::string, std::string> updates2 = {
        {"price", "12550"},
        {"arrival_time", "20:33"},
    };

    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));

    // #3 ИЗМЕНЕНИЯ
    const std::map<std::string, std::string> updates3 = {
        {"departure_date", "2015-10-25"},
        {"departure_time", "04:15"},
    };

    UPDATE_FIELD(t, departure_date, updates3);
    UPDATE_FIELD(t, departure_time, updates3);
    ASSERT_EQUAL(t.departure_time, (Time{4, 15}));
}

// void UpdateTicketExample() {
//     AirlineTicket t;
//     t.price = 5000;
//     t.from = "DME";
//     t.to = "AER";

//     const std::map<std::string, std::string> updates = {
//         {"price", "3500"},
//         {"from", "VKO"}};
//     UpdateTicket(t, updates);
//     ASSERT_EQUAL(t.from, "VKO");
//     ASSERT_EQUAL(t.to, "AER");
//     ASSERT_EQUAL(t.price, 3500);
// }

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
    // UpdateTicketExample();
}
