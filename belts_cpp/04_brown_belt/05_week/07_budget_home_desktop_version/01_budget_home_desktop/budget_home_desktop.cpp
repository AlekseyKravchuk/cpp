#include <ctime>  // tm struct
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "test_runner.h"

using namespace std;

static const int SECONDS_IN_DAY = 60 * 60 * 24;

class Date {
  public:
    Date() = default;

    friend istream& operator>>(istream& in, Date& date);
    friend ostream& operator<<(ostream& out, const Date& date);

    Date& operator++() { return *this; }

    time_t asTimeStamp() const {
        tm t;
        t.tm_sec = 0;
        t.tm_min = 0;
        t.tm_hour = 0;
        t.tm_mday = _day;
        t.tm_mon = _month - 1;
        t.tm_year = _year - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }

    int GetYear() const { return _year; }
    int GetMonth() const { return _month; }
    int GetDay() const { return _day; }

  private:
    int _year{};
    int _month{};
    int _day{};
};

istream& operator>>(istream& in, Date& date) {
    string s;
    in >> s;

    istringstream iss(s);
    iss >> date._year;
    iss.ignore(1);
    iss >> date._month;
    iss.ignore(1);
    iss >> date._day;
    return in;
}

ostream& operator<<(ostream& out, const Date& date) {
    out << setfill('0') << setw(4) << date.GetYear() << "-" << setw(2)
        << date.GetMonth() << "-" << setw(2) << date.GetDay();

    return out;
}

time_t computeDaysDiff(const Date& from, const Date& to) {
    const time_t timeStampTo = to.asTimeStamp();
    const time_t timeStampFrom = from.asTimeStamp();
    return (timeStampTo - timeStampFrom) / SECONDS_IN_DAY;
}

class BudgetManager {
  public:
    double computeIncome(const Date& from, const Date& to) {
        double commonIncome = 0.;
        auto daysCnt = computeDaysDiff(from, to) + 1;
        auto fromAsStamp = from.asTimeStamp();
        for (int i = 0; i < daysCnt; ++i) {
            commonIncome += _valByDate[fromAsStamp + i * SECONDS_IN_DAY];
        }
        return commonIncome;
    }

    void earn(const Date& from, const Date& to, int value) {
        time_t days_count = computeDaysDiff(from, to) + 1;
        double incomeByDay = static_cast<double>(value) / static_cast<double>(days_count);
        time_t fromAsStamp = from.asTimeStamp();
        for (int i = 0; i < days_count; ++i) {
            _valByDate[fromAsStamp + i * SECONDS_IN_DAY] += incomeByDay;
        }
    }

    void payTax(const Date& from, const Date& to) {
        time_t days_count = computeDaysDiff(from, to) + 1;
        time_t fromAsStamp = from.asTimeStamp();
        for (int i = 0; i < days_count; ++i) {
            _valByDate[fromAsStamp + i * SECONDS_IN_DAY] *= 0.87;
        }
    }

  private:
    unordered_map<time_t, double> _valByDate;
};

// ======================= TESTS =======================
void TestReadDate() {
    string str("2020-07-22");
    istringstream input(str);
    Date date;
    input >> date;

    ostringstream out;
    out << date;
    ASSERT_EQUAL(out.str(), str);
}

void TestAll() {
    cout.precision(25);
    TestRunner tr;
    RUN_TEST(tr, TestReadDate);
}
// ==================== END OF TESTS ====================

int main() {
    TestAll();

    cout.precision(25);
    BudgetManager budget;
    int queryCnt = 0;
    cin >> queryCnt;
    for (int i = 0; i < queryCnt; ++i) {
        string queryType;
        cin >> queryType;

        Date from, to;
        cin >> from >> to;
        if (queryType == "ComputeIncome") {
            double value = budget.computeIncome(from, to);
            cout << setprecision(25) << value << '\n';
        } else if (queryType == "Earn") {
            int value;
            cin >> value;
            budget.earn(from, to, value);
        } else if (queryType == "PayTax") {
            budget.payTax(from, to);
        }
    }

    return 0;
}