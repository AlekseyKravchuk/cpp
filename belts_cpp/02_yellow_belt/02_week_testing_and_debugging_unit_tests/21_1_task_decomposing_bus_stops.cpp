#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>  // std::istringstream
#include <string>
#include <tuple>
#include <vector>
#include <utility>  // std::pair

using namespace std;

// #define _GLIBCXX_DEBUG 1  // включить режим отладки

/*
===================== Декомпозиция программы =====================

Введение

В курсе «Белый пояс по С++» у нас была задача «Автобусные остановки — 1».
В этой задаче вам будет дано её правильное решение, целиком содержащееся внутри функции main.
Вам надо будет выполнить декомпозицию этого решения на заранее заданные блоки так,
чтобы получившаяся программа так же корректно решала задачу.
Условие задачи «Автобусные остановки — 1» приведено ниже.

Исходные файлы:
    - Авторское решение этой задачи содержится в файле "correct.cpp" ("21_task_stepik_solution.cpp")
    - Кроме того, вам дан файл "starter.cpp", который содержит заготовки классов и функций. Не меняя функцию main,
      вам надо реализовать эти классы и функции так, чтобы получившаяся программа решала задачу «Автобусные остановки — 1».

Как будет тестироваться ваша программа:

Автоматическая проверяющая система заменит в вашей программе функцию main на ту, которая дана вам в файле starter.cpp, скомпилирует получившийся файл и прогонит на тестах для задачи «Автобусные остановки — 1».
Условие задачи «Автобусные остановки — 1»

Реализуйте систему хранения автобусных маршрутов. Вам нужно обрабатывать следующие запросы:

    NEW_BUS bus stop_count stop1 stop2 ... — добавить маршрут автобуса с названием bus и stop_count остановками с названиями stop1, stop2, ...

    BUSES_FOR_STOP stop — вывести названия всех маршрутов автобуса, проходящих через остановку stop.

    STOPS_FOR_BUS bus — вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок.

    ALL_BUSES — вывести список всех маршрутов с остановками.

Формат ввода

В первой строке ввода содержится количество запросов Q, затем в Q строках следуют описания запросов.

Гарантируется, что все названия маршрутов и остановок состоят лишь из латинских букв, цифр и знаков подчёркивания.

Для каждого запроса NEW_BUS bus stop_count stop1 stop2 ... гарантируется, что маршрут bus отсутствует, количество остановок больше 0, а после числа stop_count следует именно такое количество названий остановок, причём все названия в каждом списке различны.
Формат вывода

Для каждого запроса, кроме NEW_BUS, выведите соответствующий ответ на него:

    На запрос BUSES_FOR_STOP stop выведите через пробел список автобусов, проезжающих через эту остановку, в том порядке, в котором они создавались командами NEW_BUS. Если остановка stop не существует, выведите No stop.

    На запрос STOPS_FOR_BUS bus выведите описания остановок маршрута bus в отдельных строках в том порядке, в котором они были заданы в соответствующей команде NEW_BUS. Описание каждой остановки stop должно иметь вид Stop stop: bus1 bus2 ..., где bus1 bus2 ... — список автобусов, проезжающих через остановку stop, в порядке, в котором они создавались командами NEW_BUS, за исключением исходного маршрута bus. Если через остановку stop не проезжает ни один автобус, кроме bus, вместо списка автобусов для неё выведите no interchange. Если маршрут bus не существует, выведите No bus.

    На запрос ALL_BUSES выведите описания всех автобусов в алфавитном порядке. Описание каждого маршрута bus должно иметь вид Bus bus: stop1 stop2 ..., где stop1 stop2 ... — список остановок автобуса bus в порядке, в котором они были заданы в соответствующей команде NEW_BUS. Если автобусы отсутствуют, выведите No buses.

Предупреждение:

Условие задачи выше содержит много важных деталей. Если вы не уверены в том, что не упустили ни одной, перечитайте условие ещё раз.
Пример

Ввод:
10
ALL_BUSES
BUSES_FOR_STOP Marushkino
STOPS_FOR_BUS 32K
NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo
NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
BUSES_FOR_STOP Vnukovo
NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo
STOPS_FOR_BUS 272
ALL_BUSES

Вывод:
No buses
No stop
No bus
32 32K
Stop Vnukovo: 32 32K 950
Stop Moskovsky: no interchange
Stop Rumyantsevo: no interchange
Stop Troparyovo: 950
Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo
Bus 32: Tolstopaltsevo Marushkino Vnukovo
Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
*/

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
            int num_stops;
            is >> q.bus >> num_stops;
            q.stops.resize(num_stops);
            for (int i = 0; i < num_stops; ++i) {
                is >> q.stops[i];
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
    return (!r.buses_for_stop.empty()) ? os << Join(r.buses_for_stop) : os << "No stop"s;
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
