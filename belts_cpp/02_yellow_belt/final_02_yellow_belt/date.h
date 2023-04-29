#include <sstream>
#include <string>

using namespace std::literals;

class Date {
   public:
    Date(int new_year, int new_month, int new_day);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;

   private:
    int _year;
    int _month;
    int _day;
};

Date ParseDate(const std::string date_str);