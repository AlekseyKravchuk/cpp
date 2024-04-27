#include <algorithm>  // std::max_element
#include <iostream>
#include <memory>

#include "parse.h"
#include "tests.h"
#include "transport_guide.h"

using namespace std;

int main() {
    TestAll();

    TransportGuide guide;

    // Мы не можем одновременненно формировать и структуры данных с остановками, и структуры данных с маршрутами,
    // состоящими из этих остановок. Нам сначала нужно сформировать структуры данных из остановок, и только потом
    // на их основе формировать структуры данных с маршрутами.
    vector<string> buffer;
    buffer.reserve(MAX_CREATE_QUERIES);

//    auto [stops, stop_name_to_stop_ptr] = ParseStopQueries(std::cin, buffer);
//    guide.SetStops({std::move(stops), std::move(stop_name_to_stop_ptr)});
    guide.SetStops(ParseStopQueries(std::cin, buffer));

    // TODO: implement function:
    //  BusInfo ParseBusQueries(vector<string>& buffer);
//    auto [buses, bus_name_to_bus_ptr] = ParseBusQueries(buffer);


    return 0;
}