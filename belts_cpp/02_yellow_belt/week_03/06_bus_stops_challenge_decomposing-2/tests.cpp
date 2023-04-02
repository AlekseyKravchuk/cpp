#include "tests.h"

bool operator==(const Query& lhs, const Query& rhs) {
    return std::tuple(static_cast<int>(lhs.type), lhs.bus, lhs.stop, lhs.stops) ==
           std::tuple(static_cast<int>(rhs.type), rhs.bus, rhs.stop, rhs.stops);
}

void Test_InputProcessing() {
    std::string path = "input.txt"s;

    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }

    {
        RedirectStandardInput redirection(input);

        int query_count;
        Query q;

        std::cin >> query_count;
        std::vector<Query> queries(query_count);

        for (int i = 0; i < query_count; ++i) {
            std::cin >> queries[i];
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

        std::map<std::string, std::vector<std::string>> expected_buses_to_stops = {
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

        std::string out_str_expected =
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

        std::string out_str_expected = "No buses"s;

        assert(oss.str() == out_str_expected);
    }

    std::cout << "Test_OstreamOperator_AllBusesResponse PASSED"s << std::endl;
}

void Test_GetBusesForStop() {
    {
        BusManager bm;
        std::string stop = "Moskovsky"s;

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

        std::string stop = "Vnukovo"s;
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

        std::string stop = "Moskovsky"s;
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

        std::string bus = "272"s;
        std::ostringstream oss;
        oss << bm.GetStopsForBus(bus);

        std::string expected_response =
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

// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(Test_InputProcessing, "Test_InputProcessing"s);
    tr.RunTest(Test_AddBus_GetAllBuses, "Test_AddBus_GetAllBuses"s);
    tr.RunTest(Test_OstreamOperator_AllBusesResponse, "Test_OstreamOperator_AllBusesResponse"s);
    tr.RunTest(Test_GetBusesForStop, "Test_GetBusesForStop"s);
    tr.RunTest(Test_OstreamOperator_BusesForStopResponse, "Test_OstreamOperator_BusesForStopResponse"s);
    tr.RunTest(Test_GetStopsForBus, "Test_GetStopsForBus"s);
    tr.RunTest(Test_OstreamOperator_StopsForBusResponse, "Test_OstreamOperator_StopsForBusResponse"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll
