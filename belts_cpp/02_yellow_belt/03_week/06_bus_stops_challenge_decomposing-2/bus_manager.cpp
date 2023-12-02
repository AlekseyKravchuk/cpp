#include "bus_manager.h"

void BusManager::AddBus(const std::string& bus, const std::vector<std::string>& stops) {
    for (const auto& stop : stops) {
        _buses_to_stops[bus].push_back(stop);
        _stops_to_buses[stop].push_back(bus);
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string& stop) const {
    return (_stops_to_buses.count(stop)) ? BusesForStopResponse{_stops_to_buses.at(stop)} : BusesForStopResponse();
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string& bus) const {
    StopsForBusResponse response;

    // если маршрут "bus" ранее был ранее добавлен,
    // И если "_stops_to_buses[stop_name] == 1",
    // то через эту остановку проходит ТОЛЬКО этот маршрут, => "no interchange"
    if (_buses_to_stops.count(bus)) {
        for (const auto& stop_name : _buses_to_stops.at(bus)) {
            std::pair<std::string, std::vector<std::string>> interchanges_for_stop;
            interchanges_for_stop.first = stop_name;
            for (const auto& other_bus : _stops_to_buses.at(stop_name)) {
                if (_stops_to_buses.at(stop_name).size() == 1) {
                    interchanges_for_stop.second = {"no interchange"s};
                    break;
                }

                if (other_bus != bus) {
                    interchanges_for_stop.second.push_back(other_bus);
                }
            }
            response.stops_for_bus.push_back(interchanges_for_stop);
        }
    }
    return response;
}

AllBusesResponse BusManager::GetAllBuses() const {
    return AllBusesResponse{_buses_to_stops};
}

