#pragma once

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>

using namespace std::literals;

class Date {
   public:
    Date(int new_year, int new_month, int new_day);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;

    bool operator<(const Date& rhs) const;
    bool operator<=(const Date& rhs) const;
    bool operator>(const Date& rhs) const;
    bool operator>=(const Date& rhs) const;
    bool operator==(const Date& rhs) const;
    bool operator!=(const Date& rhs) const;

   private:
    int _year;
    int _month;
    int _day;
};

Date ParseDate(std::istringstream& iss);

std::ostream& operator<<(std::ostream& os, const Date& date);
