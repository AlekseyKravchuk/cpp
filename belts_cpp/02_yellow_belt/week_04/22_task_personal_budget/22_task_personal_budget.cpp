#include <algorithm>  // std::max
#include <fstream>    // for input redirection
#include <iomanip>
#include <iostream>
#include <numeric>  // std::accumulate
#include <sstream>
#include <vector>

using namespace std::literals;

#define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input) {
        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
#endif  //_GLIBCXX_DEBUG

class Date {
   public:
    Date() = default;

    Date(const std::string& date) {
        std::stringstream ss(date);
        ss >> _year;
        ss.ignore(1);
        ss >> _month;
        ss.ignore(1);
        ss >> _day;

        _total_days = GetDaysFromEpoch();
    }

    int GetTotalDaysFromEpoch() const { return _total_days; }

    friend std::istream& operator>>(std::istream& is, Date& date);

    int GetDaysFromEpoch() {
        int year = _year - _year_start;
        int days_count = 0;
        days_count += year * 365;

        // проверяем, нужно ли учитывать текущий год в подсчете вискокосных годов
        if (_month < 2 && year != 0) {
            --year;
        }

        days_count += daysUpToCurrentMonth[_month] + _day;
        days_count += LeapYearsCount(year);
        // ++days_count;  // по условию задачи дипазон дат симметричный => добавляем единицу

        return days_count;
    }

    bool operator<=(const Date& rhs) const {
        return this->_total_days <= rhs._total_days;
    }

    bool operator<(const Date& rhs) const {
        return this->_total_days < rhs._total_days;
    }

    int operator-(const Date& rhs) {
        return this->_total_days - rhs._total_days + 1;
    }

   private:
    int _year = 1500;
    int _month = 1;
    int _day = 1;
    int _total_days = 0;  // количество дней начиная с "_year_start"
    static const int _year_start = 1500;
    static constexpr const int daysUpToCurrentMonth[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    // Определяет количество високосных годов в диапазоне [_year_start, year]
    int LeapYearsCount(int year) {
        // год является високосным, если он:
        // 1) делится на 4, но не делится на 100;
        // 2) кратен 400.
        return (year / 4) - (year / 100) + (year / 400);
    }
};

// Time-Earning Interval
class TEInterval {
   public:
    TEInterval() = default;

    TEInterval(const Date& start, const Date& end) : _start(start), _end(end) {
        if (end < start) {
            std::swap(_start, _end);
        }
        // по условию задачи обе даты из диапазона [_start, _end] должны быть включены в диапазон => +1
        _duration_in_days = _end.GetTotalDaysFromEpoch() - _start.GetTotalDaysFromEpoch() + 1;
    }

    TEInterval(const Date& start, const Date& end, double income) : TEInterval(start, end) {
        _earning_per_day = income / _duration_in_days;
    }

    const Date& Start() const { return _start; }
    const Date& End() const { return _end; }

    double GetEarningPerDay() const {
        return _earning_per_day;
    }

   private:
    Date _start{};
    Date _end{};
    int _duration_in_days{};
    double _earning_per_day{};
};

std::istream& operator>>(std::istream& is, Date& date) {
    is >> date._year;
    is.ignore(1);
    is >> date._month;
    is.ignore(1);
    is >> date._day;
    date._total_days = date.GetDaysFromEpoch();

    return is;
}

class Budget {
   public:
    void AddEarning(const Date& date_from, const Date& date_to, double income) {
        _earnings.emplace_back(date_from, date_to, income);
    }

    double ComputeIncome(const TEInterval& sample) {
        return std::accumulate(_earnings.begin(), _earnings.end(), 0.0,
                               [&sample](double acc, const TEInterval& earned) -> double {
                                   auto max_start = std::max(sample.Start(), earned.Start());
                                   auto min_end = std::min(sample.End(), earned.End());

                                   // если временные интервалы пересекаются
                                   if (max_start <= min_end) {
                                       auto tmp = acc + (min_end - max_start) * earned.GetEarningPerDay();
                                       return tmp;
                                   } else {  // если у временных интервалов нет точек пересечения
                                       return acc;
                                   }
                               });
    }

    void ProcessQuery(const std::string& line) {
        std::string query_type;
        std::stringstream ss(line);
        Date date_from, date_to;
        
        ss >> query_type;
        if (query_type == "Earn"s) {
            double income;
            ss >> date_from >> date_to >> income;
            AddEarning(date_from, date_to, income);
        } else if (query_type == "ComputeIncome"s) {
            ss >> date_from >> date_to;
            auto tmp = ComputeIncome({date_from, date_to});
            std::cout << std::setprecision(25) << tmp << std::endl;
        }
    }

   private:
    std::vector<TEInterval> _earnings;
};

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "22_input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    int n{};
    std::cin >> n >> std::ws;

    Budget budget;
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);
        budget.ProcessQuery(line);
    }

    return 0;
}
