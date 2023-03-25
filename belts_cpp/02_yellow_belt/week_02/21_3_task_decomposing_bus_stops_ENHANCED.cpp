#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>  // std::istringstream
#include <string>
#include <vector>
#include <utility>  // std::pair

using namespace std;

#define _GLIBCXX_DEBUG 1  // включить режим отладки

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

template <typename Collection>
std::string Join(const Collection& collection, const std::string separator = " "s) {
    std::ostringstream oss;
    bool isFirst = true;
    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << separator;
        }
        isFirst = false;
        oss << elm;
    }

    return oss.str();
}

map<string, QueryType> str2qtype = {
    {"NEW_BUS"s, QueryType::NewBus},
    {"BUSES_FOR_STOP"s, QueryType::BusesForStop},
    {"STOPS_FOR_BUS"s, QueryType::StopsForBus},
    {"ALL_BUSES"s, QueryType::AllBuses},
};

istream& operator>>(istream& is, QueryType& qtype) {
    string type_as_str;
    is >> type_as_str;
    qtype = str2qtype[type_as_str];

    return is;
}

istream& operator>>(istream& is, Query& q) {
    is >> std::ws;
    if (is.peek() == '\n') {
        is.ignore(1);
    }
    is >> q.type;
    switch (q.type) {
        case QueryType::NewBus: {
            int stop_count = 0;
            is >> q.bus >> stop_count;
            q.stops.resize(stop_count);
            for (auto& stop : q.stops) {
                is >> stop;
            }
            break;
        }
        case QueryType::BusesForStop:
            is >> q.stop;
            break;
        case QueryType::StopsForBus:
            is >> q.bus;
            break;
        case QueryType::AllBuses:
            // code
            break;
    }

    return is;
}

struct BusesForStopResponse {
    vector<string> buses_for_stop;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    return (r.buses_for_stop.empty()) ? os << "No stop"s : os << Join(r.buses_for_stop);
}

struct StopsForBusResponse {
    vector<pair<string, vector<string>>> stops_for_bus;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.stops_for_bus.empty()) {
        os << "No bus"s;
    } else {
        bool isFirst = true;
        for (const auto& [stop, buses_list] : r.stops_for_bus) {
            if (!isFirst) {
                os << endl;
            }
            isFirst = false;
            os << "Stop " << stop << ": "s << Join(buses_list);
        }
    }
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> response;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.response.empty()) {
        os << "No buses"s;
    } else {
        bool isFirst = true;
        for (const auto& [bus, stop_names] : r.response) {
            if (!isFirst) {
                os << std::endl;
            }
            isFirst = false;
            os << "Bus "s << bus << ": " << Join(stop_names);
        }
    }
    return os;
}

class BusManager {
   public:
    void AddBus(const string& bus, const vector<string>& stops) {
        for (const auto& stop : stops) {
            _buses_to_stops[bus].push_back(stop);
            _stops_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        return (_stops_to_buses.count(stop)) ? BusesForStopResponse{_stops_to_buses.at(stop)} : BusesForStopResponse();
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse response;

        // если маршрут "bus" ранее был ранее добавлен,
        // И если "_stops_to_buses[stop_name] == 1",
        // то через эту остановку проходит ТОЛЬКО этот маршрут, => "no interchange"
        if (_buses_to_stops.count(bus)) {
            for (const auto& stop_name : _buses_to_stops.at(bus)) {

                pair<string, vector<string>> interchanges_for_stop;
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

    AllBusesResponse GetAllBuses() const {
        return AllBusesResponse{_buses_to_stops};
    }

   private:
    map<string, vector<string>> _buses_to_stops;
    map<string, vector<string>> _stops_to_buses;
};

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

bool operator==(const Query& lhs, const Query& rhs) {
    return std::tuple(static_cast<int>(lhs.type), lhs.bus, lhs.stop, lhs.stops) ==
           std::tuple(static_cast<int>(rhs.type), rhs.bus, rhs.stop, rhs.stops);
}

void Test_InputProcessing() {
    std::string path = "21_task_decomposing_bus_stops_input_for_test_queries.txt"s;

    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }

    {
        RedirectStandardInput redirection(input);

        int query_count;
        Query q;

        cin >> query_count;
        std::vector<Query> queries(query_count);

        for (int i = 0; i < query_count; ++i) {
            cin >> queries[i];
        }

        Query q0_expected = {QueryType::AllBuses};
        assert(q0_expected == queries[0]);

        Query q1_expected = {QueryType::BusesForStop, ""s, "Marushkino"s, {}};
        assert(q1_expected == queries[1]);

        Query q2_expected = {QueryType::StopsForBus, "32K"s, ""s, {}};
        assert(q2_expected == queries[2]);

        Query q3_expected = {QueryType::NewBus, "32"s, ""s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s}};
        assert(q3_expected == queries[3]);

        std::cout << "Test_InputProcessing PASSED" << std::endl;
    }  // по этой закрывающей скобке будет вызван деструктор ~RedirectStandardInput, который восстановит стандартный ввод
}

void Test_AddBus_GetAllBuses() {
    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        bm.AddBus("950"s, {"Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s});
        bm.AddBus("272"s, {"Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

        map<string, vector<string>> expected_buses_to_stops = {
            {"272"s, {"Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s}},
            {"32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s}},
            {"32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s}},
            {"950"s, {"Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s}}};

        assert(expected_buses_to_stops == bm.GetAllBuses().response);
    }

    std::cout << "Test_AddBus_GetAllBuses PASSED"s << std::endl;
}

void Test_OstreamOperator_AllBusesResponse() {
    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        bm.AddBus("950"s, {"Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s});
        bm.AddBus("272"s, {"Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

        std::ostringstream oss;
        oss << bm.GetAllBuses();

        string out_str_expected =
            "Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo\n"
            "Bus 32: Tolstopaltsevo Marushkino Vnukovo\n"
            "Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo\n"
            "Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo"s;

        assert(oss.str() == out_str_expected);
    }

    {
        BusManager bm;
        std::ostringstream oss;
        oss << bm.GetAllBuses();

        string out_str_expected = "No buses"s;

        assert(oss.str() == out_str_expected);
    }

    std::cout << "Test_OstreamOperator_AllBusesResponse PASSED"s << std::endl;
}

void Test_GetBusesForStop() {
    {
        BusManager bm;
        string stop = "Moskovsky"s;

        BusesForStopResponse response = bm.GetBusesForStop(stop);
        BusesForStopResponse expected_response = BusesForStopResponse{{}};
        assert(response.buses_for_stop == expected_response.buses_for_stop);
    }

    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        bm.AddBus("950"s, {"Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s});
        bm.AddBus("272"s, {"Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

        string stop = "Vnukovo"s;
        BusesForStopResponse response = bm.GetBusesForStop(stop);
        BusesForStopResponse expected_response = BusesForStopResponse{{"32"s, "32K"s, "950"s, "272"s}};
        assert(response.buses_for_stop == expected_response.buses_for_stop);
    }

    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        bm.AddBus("950"s, {"Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s});
        bm.AddBus("272"s, {"Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

        string stop = "Moskovsky"s;
        BusesForStopResponse response = bm.GetBusesForStop(stop);
        BusesForStopResponse expected_response = BusesForStopResponse{{"272"s}};
        assert(response.buses_for_stop == expected_response.buses_for_stop);
    }

    std::cout << "Test_GetBusesForStop PASSED"s << std::endl;
}

void Test_OstreamOperator_BusesForStopResponse() {
    {
        BusManager bm;
        std::ostringstream oss;
        oss << bm.GetBusesForStop("Marushkino"s);
        assert(oss.str() == "No stop"s);
    }

    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        std::ostringstream oss;
        oss << bm.GetBusesForStop("Vnukovo"s);
        assert(oss.str() == "32 32K"s);
    }

    std::cout << "Test_OstreamOperator_BusesForStopResponse PASSED"s << std::endl;
}

void Test_GetStopsForBus() {
    {
        BusManager bm;
        std::ostringstream oss;
        StopsForBusResponse res = bm.GetStopsForBus("32K"s);
        StopsForBusResponse expected_res = StopsForBusResponse{};
        assert(res.stops_for_bus == expected_res.stops_for_bus);
    }

    std::cout << "Test_GetStopsForBus PASSED"s << std::endl;
}

void Test_OstreamOperator_StopsForBusResponse() {
    {
        BusManager bm;
        std::ostringstream oss;
        oss << bm.GetStopsForBus("32K"s);
        assert(oss.str() == "No bus"s);
    }

    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        bm.AddBus("950"s, {"Kokoshkino"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Troparyovo"s});
        bm.AddBus("272"s, {"Vnukovo"s, "Moskovsky"s, "Rumyantsevo"s, "Troparyovo"s});

        string bus = "272"s;
        std::ostringstream oss;
        oss << bm.GetStopsForBus(bus);

        string expected_response =
            "Stop Vnukovo: 32 32K 950\n"
            "Stop Moskovsky: no interchange\n"
            "Stop Rumyantsevo: no interchange\n"
            "Stop Troparyovo: 950"s;

        // std::cout << "response:" << std::endl;
        // std::cout << oss.str() << std::endl;
        // std::cout << "==============================="s << std::endl;

        // std::cout << "expected_response:" << std::endl;
        // std::cout << expected_response << std::endl;
        // std::cout << "==============================="s << std::endl;

        assert(oss.str() == expected_response);
    }

    std::cout << "Test_OstreamOperator_StopsForBusResponse PASSED"s << std::endl;
}

void RunTests() {
    Test_InputProcessing();
    Test_AddBus_GetAllBuses();
    Test_OstreamOperator_AllBusesResponse();
    Test_GetBusesForStop();
    Test_OstreamOperator_BusesForStopResponse();
    Test_GetStopsForBus();
    Test_OstreamOperator_StopsForBusResponse();
}

// Не меняя тела функции main, реализуйте функции и классы выше
int main() {
#ifdef _GLIBCXX_DEBUG
    RunTests();
#else
    // // std::string path = "21_task_decomposing_bus_stops_input_for_test_queries.txt"s;
    // std::string path = "21_task_decomposing_bus_stops_input.txt"s;
    // std::ifstream input(path);
    // if (!input) {
    //     throw std::runtime_error("File \""s + path + "\" is not opened"s);
    // }
    // RedirectStandardInput redirection(input);

    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
#endif  //_GLIBCXX_DEBUG

    return 0;
}
