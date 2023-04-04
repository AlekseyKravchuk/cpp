#include "tests.h"

bool operator==(const Query& lhs, const Query& rhs) {
    return std::tuple(static_cast<int>(lhs.type), lhs.bus, lhs.stop, lhs.stops) ==
           std::tuple(static_cast<int>(rhs.type), rhs.bus, rhs.stop, rhs.stops);
}

bool operator!=(const Query& lhs, const Query& rhs) {
    return !(lhs == rhs);
}

void Test_Pair_Vector_Map_Set_Ouput() {
    {
        std::ostringstream oss;
        std::string desired_str = "2: 15"s;
        std::pair<int, int> p{2, 15};
        oss << p;
        AssertEqual(desired_str, oss.str(), "test pair output"s);
    }

    {
        std::ostringstream oss;
        std::string desired_str = "[6, 4, 12, 5, 11]"s;
        std::vector<int> v{6, 4, 12, 5, 11};
        oss << v;
        AssertEqual(desired_str, oss.str(), "test vector output"s);
    }

    {
        std::ostringstream oss;
        std::string desired_str = "{4, 5, 6, 11, 12}"s;
        std::set<int> s{6, 4, 12, 5, 11};
        oss << s;
        AssertEqual(desired_str, oss.str(), "test set output"s);
    }

    {
        std::ostringstream oss;
        std::string desired_str = "{4: four, 5: five, 6: six}"s;
        std::map<int, std::string> m = {
            {4, "four"s},
            {5, "five"s},
            {6, "six"s}
            };
        oss << m;
        AssertEqual(desired_str, oss.str(), "test set output"s);
    }
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

        Query q0_expected = Query{QueryType::AllBuses, {}, {}, {}};
        AssertEqual(q0_expected, queries[0]);

        Query q1_expected = {QueryType::BusesForStop, ""s, "Marushkino"s, {}};
        AssertEqual(q1_expected, queries[1]);

        Query q2_expected = {QueryType::StopsForBus, "32K"s, ""s, {}};
        AssertEqual(q2_expected, queries[2]);

        Query q3_expected = {QueryType::NewBus, "32"s, ""s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s}};
        AssertEqual(q3_expected, queries[3]);
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

        AssertEqual(expected_buses_to_stops, bm.GetAllBuses().response);
    }
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

        AssertEqual(oss.str(), out_str_expected);
    }

    {
        BusManager bm;
        std::ostringstream oss;
        oss << bm.GetAllBuses();

        std::string out_str_expected = "No buses"s;

        AssertEqual(oss.str(), out_str_expected);
    }
}

void Test_GetBusesForStop() {
    {
        BusManager bm;
        std::string stop = "Moskovsky"s;

        BusesForStopResponse response = bm.GetBusesForStop(stop);
        BusesForStopResponse expected_response = BusesForStopResponse{{}};
        AssertEqual(response.buses_for_stop, expected_response.buses_for_stop);
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
        AssertEqual(response.buses_for_stop, expected_response.buses_for_stop);
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
        AssertEqual(response.buses_for_stop, expected_response.buses_for_stop);
    }
}

void Test_OstreamOperator_BusesForStopResponse() {
    {
        BusManager bm;
        std::ostringstream oss;
        oss << bm.GetBusesForStop("Marushkino"s);
        AssertEqual(oss.str(), "No stop"s);
    }

    {
        BusManager bm;
        bm.AddBus("32"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
        bm.AddBus("32K"s, {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
        std::ostringstream oss;
        oss << bm.GetBusesForStop("Vnukovo"s);
        AssertEqual(oss.str(), "32 32K"s);
    }
}

void Test_GetStopsForBus() {
    {
        BusManager bm;
        std::ostringstream oss;
        StopsForBusResponse res = bm.GetStopsForBus("32K"s);
        StopsForBusResponse expected_res = StopsForBusResponse{};
        AssertEqual(res.stops_for_bus, expected_res.stops_for_bus);
    }
}

void Test_OstreamOperator_StopsForBusResponse() {
    {
        BusManager bm;
        std::ostringstream oss;
        oss << bm.GetStopsForBus("32K"s);
        AssertEqual(oss.str(), "No bus"s);
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

        AssertEqual(oss.str(), expected_response);
    }
}

// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(Test_Pair_Vector_Map_Set_Ouput, "Test_Pair_Vector_Map_Set_Ouput"s);
    tr.RunTest(Test_InputProcessing, "Test_InputProcessing"s);
    tr.RunTest(Test_AddBus_GetAllBuses, "Test_AddBus_GetAllBuses"s);
    tr.RunTest(Test_OstreamOperator_AllBusesResponse, "Test_OstreamOperator_AllBusesResponse"s);
    tr.RunTest(Test_GetBusesForStop, "Test_GetBusesForStop"s);
    tr.RunTest(Test_OstreamOperator_BusesForStopResponse, "Test_OstreamOperator_BusesForStopResponse"s);
    tr.RunTest(Test_GetStopsForBus, "Test_GetStopsForBus"s);
    tr.RunTest(Test_OstreamOperator_StopsForBusResponse, "Test_OstreamOperator_StopsForBusResponse"s);
}  // По этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner",
   // в котором произойдет обработка количества упавших тестов.
   // Для своевременного вызова деструктора "TestRunner::~TestRunner" мы и используем функцию TestAll
