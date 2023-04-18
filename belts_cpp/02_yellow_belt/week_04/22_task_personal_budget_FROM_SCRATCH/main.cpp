#include <chrono>
#include <ctime>    // std::mktime
#include <iomanip>  // std::get_time
#include <iostream>
#include <sstream>
#include <string>

using namespace std::literals;

int main() {
    // std::stringstream ss("2000-01-02 2000-01-06"s);
    std::stringstream ss("2000-03-02 2099-03-02"s);
    std::tm date_from_tm{};
    std::tm date_to_tm{};
    ss >> std::get_time(&date_from_tm, "%Y-%m-%d") >> std::get_time(&date_to_tm, "%Y-%m-%d");

    std::time_t date_from_time_t = std::mktime(&date_from_tm);
    std::time_t date_to_time_t = std::mktime(&date_to_tm);

    if (date_from_time_t != (std::time_t)(-1) && date_to_time_t != (std::time_t)(-1)) {
        double difference = std::difftime(date_to_time_t, date_from_time_t) / (60 * 60 * 24) + 1;
        std::cout << "difference = " << difference << " days" << std::endl;
    }

    return 0;
}
