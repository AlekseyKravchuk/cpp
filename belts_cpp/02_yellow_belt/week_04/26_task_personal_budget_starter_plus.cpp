#include <algorithm>  // std::max
#include <fstream>    // for input redirection
#include <iomanip>
#include <iostream>
#include <numeric>  // std::accumulate
#include <sstream>
#include <vector>

using namespace std::literals;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

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

    friend std::istream& operator>>(std::istream& is, Date& date);

    int Days() {
        int year = _year - _year_start;
        int days_count = 0;
        days_count += year * 365;

        // проверяем, нужно ли учитывать текущий год в подсчете вискокосных годов
        if (_month < 2 && year != 0) {
            --year;
        }

        days_count += daysUpToCurrentMonth[_month] + _day;
        days_count += LeapYearsCount(year);

        return days_count;
    }

   private:
    int _year{};
    int _month{};
    int _day{};
    int _total_days{};  // количество дней начиная с "_year_start"
    static const int _year_start = 1700;
    static constexpr const int daysUpToCurrentMonth[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    // Определяет количество високосных годов в диапазоне [_year_start, year]
    int LeapYearsCount(int year) {
        // год является високосным, если он:
        // 1) делится на 4, но не делится на 100;
        // 2) кратен 400.
        return (year / 4) - (year / 100) + (year / 400);
    }
};

std::istream& operator>>(std::istream& is, Date& date) {
    is >> date._year;
    is.ignore(1);
    is >> date._month;
    is.ignore(1);
    is >> date._day;
    date._total_days = date.Days();

    return is;
}

class Budget {
   public:
    void AddEarning(const std::string& line) {
        std::stringstream ss(line);
        Date date;
        double income;
        ss >> date >> income;
        _earnings[date.Days()] += income;
    }

    void ProcessQuery(const std::string& line) {
        std::stringstream ss(line);
        Date date_from{}, date_to{};
        ss >> date_from >> date_to;

        std::cout
            << std::setprecision(25)
            << _earnings[date_to.Days()] - _earnings[date_from.Days() - 1]
            << std::endl;
    }

    void CalcPartialSums() {
        std::partial_sum(std::begin(_earnings),
                         std::end(_earnings),
                         std::begin(_earnings));
    }

   private:
    double _earnings[500 * 365];
};

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "26_input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    int n_earnings{}, n_queries{};
    Budget budget;

    std::cin >> n_earnings >> std::ws;
    for (int i = 0; i < n_earnings; ++i) {
        std::string line;
        std::getline(std::cin, line);
        budget.AddEarning(line);
    }

    budget.CalcPartialSums();

    std::cin >> n_queries >> std::ws;
    for (int j = 0; j < n_queries; ++j) {
        std::string line;
        std::getline(std::cin, line);
        budget.ProcessQuery(line);
    }

    return 0;
}
