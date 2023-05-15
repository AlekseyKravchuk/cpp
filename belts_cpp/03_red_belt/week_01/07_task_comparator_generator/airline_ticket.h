#pragma once

#include <string>

struct Date {
    int year{};
    int month{};
    int day{};
};

struct Time {
    int hours;
    int minutes;
};

struct AirlineTicket {
    std::string from{};
    std::string to{};
    std::string airline{};
    Date departure_date{};
    Time departure_time{};
    Date arrival_date{};
    Time arrival_time{};
    int price{};
};
