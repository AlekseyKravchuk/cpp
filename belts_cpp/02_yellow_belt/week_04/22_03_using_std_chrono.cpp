#include <chrono>
#include <ctime>    // std::mktime
#include <iomanip>  // std::get_time
#include <iostream>
#include <sstream>
#include <string>

using namespace std::literals;
using namespace std::chrono;

int get_days_in_year(const std::string& date_from, const std::string& date_to) {
    using namespace std;
    using namespace std::chrono;

    // Create start time span
    std::tm tm_start = {};
    std::stringstream ss1(date_from);
    ss1 >> std::get_time(&tm_start, "%Y-%m-%d");
    std::time_t start_since_1970 = std::mktime(&tm_start);  // since 00:00, Jan 1 1970 UTC, corresponding to POSIX time
    // auto tms = system_clock::from_time_t(start_since_1970);
    std::chrono::time_point tms = system_clock::from_time_t(start_since_1970);

    // Create end time span
    std::tm tm_end = {};
    std::stringstream ss2(date_to);
    ss2 >> std::get_time(&tm_end, "%Y-%m-%d");
    std::time_t end_since_epoch = std::mktime(&tm_end);
    auto tme = system_clock::from_time_t(end_since_epoch);

    // Calculate time duration between those two dates
    auto diff_in_days = std::chrono::duration_cast<days>(tme - tms);

    return diff_in_days.count();
}

int main() {
    std::string date_from = "2000-01-01"s;
    std::string date_to = "2000-01-03";
    auto days_count = get_days_in_year(date_from, date_to);
    std::cout << "There are "s << days_count << " days between "s << date_from << " and "s << date_to << std::endl;
}
