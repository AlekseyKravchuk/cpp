#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>  // std::pair
#include <vector>
#include <ios>  // std::boolalpha

using namespace std::literals;

class Cities {
   public:
    std::tuple<bool, std::string> FindCountry(const std::string city_name) const {
        if (_city2country.count(city_name)) {
            return { true, _city2country.at(city_name)};
        } else if (_ambiguous_cities.count(city_name)) {
            return {false, "Ambiguous"s};
        } else {
            return {false, "Not exist"s};
        }
    }

   private:
    std::map<std::string, std::string> _city2country;
    std::set<std::string> _ambiguous_cities;
};

int main() {
    // Кортежи и пары нужно использовать аккуратно - они часто мешают читаемости кода

    Cities cities;
    // bool success;
    // std::string message;

    // "std::tie" создает кортеж, который хранит ссылки
    // tie(success, message) = cities.FindCountry("Volgograd"s);

    // используем structured bindings (аналог распаковки кортежа в Python)
    auto [success, message] = cities.FindCountry("Volgograd"s);

    std::cout << std::boolalpha << success << ", " << message << std::endl;
    return 0;
}
