#include <iterator>
#include <vector>

#include "airport_counter.h"
#include "test_runner.h"

int main() {
    enum class MoscowAirport {
        VKO,
        SVO,
        DME,
        ZIA,
        Last_
    };

    const std::vector<MoscowAirport> airports = {
        MoscowAirport::SVO,
        MoscowAirport::VKO,
        MoscowAirport::ZIA,
        MoscowAirport::SVO,
    };

    AirportCounter<MoscowAirport> airport_counter(std::begin(airports), std::end(airports));

    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::VKO), 1u);
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::SVO), 2u);
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::DME), 0u);
    ASSERT_EQUAL(airport_counter.Get(MoscowAirport::ZIA), 1u);

    // using Item = AirportCounter<MoscowAirport>::Item;
    // std::vector<Item> items;
    // for (const auto& item : airport_counter.GetItems()) {
    //     items.push_back(item);
    // }
    // ASSERT_EQUAL(items.size(), 4);

    return 0;
}