#include <cstdint>  // uint64_t
#include <iostream>
#include <limits>
#include <set>
#include <sstream>

using namespace std::literals;

class Date {
   public:
    Date() = default;

    Date(const std::string& date) {
        std::stringstream ss(date);
        std::string year_str{}, month_str{}, day_str{};
        if (ss && std::getline(ss, year_str, '-') && std::getline(ss, month_str, '-') && std::getline(ss, day_str)) {
            _year = std::stoi(year_str);
            _month = std::stoi(month_str);
            _day = std::stoi(day_str);
            _total_days = GetDaysFromEpoch();
        } else {
            throw std::runtime_error("Wrong date format. Correct format is: YYYY-MM-DD"s);
        }
    }

    int GetYear() const { return _year; }
    int GetMonth() const { return _month; }
    int GetDay() const { return _day; }
    int GetTotalDays() const { return _total_days; }

    void SetYear(int year) { _year = year; }
    void SetMonth(int month) { _month = month; }
    void SetDay(int day) { _day = day; }
    void SetTotalDays(int total_days) { _total_days = total_days; }

    int GetDaysFromEpoch() {
        int year = _year - _year_start;
        int days_count = 0;
        days_count += year * 365;

        // проверяем, нужно ли учитывать текущий год в подсчете вискокосных годов
        if (_month < 2) {
            --year;
        }

        days_count += daysUpToCurrentMonth[_month] + _day;
        days_count += GetLeapYearsCount(year);
        ++days_count;  // по условию задачи дипазон дат симметричный => добавляем единицу

        return days_count;
    }

   private:
    int _year = 1600;
    int _month = 1;
    int _day = 1;
    int _total_days = 0;  // количество дней начиная с "_year_start"
    static const int _year_start = 1600;
    static constexpr const int daysUpToCurrentMonth[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    // Определяет количество високосных годов в диапазоне [1600, year]
    int GetLeapYearsCount(int year) {
        // год является
        return (year / 4) - (year / 100) + (year / 400);
    }
};

bool operator<(const Date& lhs, const Date& rhs) {
    return lhs.GetTotalDays() < rhs.GetTotalDays();
}

class TInterval {
   public:
    int GetIntervalLength() const { return _duration_in_days; }
    const Date& Start() const { return _start; }
    const Date& End() const { return _end; }

    void SetIntervalLength() {
        // по условию задачи обе даты из диапазона [_start, _end] включены в диапазон => +1
        _duration_in_days = _end.GetTotalDays() - _start.GetTotalDays() + 1;
    }

    friend std::istream& operator>>(std::istream& is, TInterval& interval);

   private:
    Date _start{};
    Date _end{};
    int _duration_in_days{};
};

bool operator<(const TInterval& lhs, const TInterval& rhs) {
    // если интервалы не перекрываются слева
    if (lhs.Start() < rhs.Start() && lhs.End() < rhs.Start()) {
        return true;
    } else {  // если интервалы перекрываются слева
        return lhs.Start() < rhs.Start();
    }
}

std::istream& operator>>(std::istream& is, Date& date) {
    std::string day{}, month{}, year{};

    if (is && std::getline(is, year, '-') && std::getline(is, month, '-') && std::getline(is, day, ' ')) {
        date.SetYear(std::stoi(year));
        date.SetMonth(std::stoi(month));
        date.SetDay(std::stoi(day));

        int total_days = date.GetDaysFromEpoch();
        date.SetTotalDays(total_days);
    } else {
        throw std::runtime_error("Wrong date format. Correct format is: YYYY-MM-DD"s);
    }

    return is;
}

std::istream& operator>>(std::istream& is, TInterval& interval) {
    if (is) {
        is >> interval._start;
        is >> interval._end;
        interval.SetIntervalLength();
    }

    return is;
}

int main() {
    // std::string date_str = "2000-04-01"s;
    // std::string date_str = "2000-01-09"s;

    std::string date_from_str = "2000-01-02"s;
    std::string date_to_str = "2000-01-06"s;

    std::stringstream oss("2000-01-02 2000-01-06 20"s);
    TInterval ti_1;
    int income;
    oss >> ti_1 >> income;

    std::set<TInterval> intervals;

    // Date date_from(date_from_str);
    // Date date_to(date_to_str);

    // int delta = GetDaysDelta(date_from, date_to);
    // std::cout << delta << std::endl;

    return 0;
}
