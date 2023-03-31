#include "phone_number.h"

#include <sstream>
#include <stdexcept>

PhoneNumber::PhoneNumber(const string &international_number) {
    std::istringstream iss(international_number);
    if (iss.peek() != '+') {
        std::ostringstream oss;
        oss << "Wrong format: \""s << international_number << "\". Correct format starts from '+' sign."s;
        throw std::invalid_argument(oss.str());
    }
    iss.ignore(1);

    char delim = '-';

    string country_code;
    std::getline(iss, country_code, delim);
    if (country_code.empty()) {
        std::ostringstream oss;
        oss << "Wrong format: \""s << international_number << "\". Correct format contains country code."s;
        throw std::invalid_argument(oss.str());
    } else {
        country_code_ = country_code;
    }

    string city_code;
    std::getline(iss, city_code, delim);
    if (city_code.empty()) {
        std::ostringstream oss;
        oss << "Wrong format: \""s << international_number << "\". Correct format contains city code."s;
        throw std::invalid_argument(oss.str());
    } else {
        city_code_ = city_code;
    }

    string local_number;
    std::getline(iss, local_number);
    if (local_number.empty()) {
        std::ostringstream oss;
        oss << "Wrong format: \""s << international_number << "\". Correct format contains local number."s;
        throw std::invalid_argument(oss.str());
    } else {
        local_number_ = local_number;
    }
}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    std::ostringstream oss;
    oss << "+"s << country_code_ << "-"s << city_code_ << "-"s << local_number_;
    return oss.str();
}