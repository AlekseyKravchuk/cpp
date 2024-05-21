#include "parse.h"
#include "test_runner.h"
#include "tests.h"
#include "transport_guide.h"

#include <set>
#include <sstream>

using namespace std;

void TestTrimFunctions() {
    {
        string s = "                                                      ";
        string result_str = string{trim_view(s)};
        string expected_str;

        ASSERT_EQUAL(result_str.size(), 0u);
        ASSERT_EQUAL(result_str, expected_str);
    }

    {
        string s = " a ";
        string result_str = string{trim_view(s)};
        string expected_str = "a";

        ASSERT_EQUAL(result_str.size(), 1u);
        ASSERT_EQUAL(result_str, expected_str);
    }

    {
        string s =
                "       Tolstopaltsevo:  55.611087,   37.20829                     ";
        string expected_str = "Tolstopaltsevo:  55.611087,   37.20829";
        string result_str = string{trim_view(s)};
        ASSERT_EQUAL(result_str.size(), expected_str.size());
        ASSERT_EQUAL(expected_str, result_str);
    }


}

void TestSplitBy() {
    string s = "Tolstopaltsevo: 55.611087, 37.20829";
    vector<string_view> expected = {
            "Tolstopaltsevo",
            "55.611087, 37.20829"
    };

    vector<string_view> result = SplitBy(s, ':');
    ASSERT_EQUAL(result, expected);
}

void TestParseStopView() {
    {
        string s =
                "       Tolstopaltsevo:     55.611087,   37.20829                     ";
        Stop expected_stop = Stop{
                "Tolstopaltsevo",
                55.611087,
                37.20829};

        Stop result = ParseAddStopQuery(s);
        ASSERT_EQUAL(expected_stop.stop_name, result.stop_name);
        ASSERT_EQUAL(expected_stop.latitude_deg, result.latitude_deg);
        ASSERT_EQUAL(expected_stop.longitude_deg, result.longitude_deg);
    }
}

void TestAddStops() {
    istringstream iss{R"(
10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
3
Bus 256
Bus 750
Bus 751
)"};

    TransportGuide guide;
    guide.CreateDB(iss);

    vector<string> expected_buffer = {
            "256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye",
            "750: Tolstopaltsevo - Marushkino - Rasskazovka"
    };

    deque<Stop> expected_stops = {
            {"Tolstopaltsevo", 55.611087, 37.20829},
            {"Marushkino", 55.595884, 37.209755},
            {"Rasskazovka", 55.632761, 37.333324},
            {"Biryulyovo Zapadnoye", 55.574371, 37.6517},
            {"Biryusinka", 55.581065, 37.64839},
            {"Universam", 55.587655, 37.645687},
            {"Biryulyovo Tovarnaya", 55.592028, 37.653656},
            {"Biryulyovo Passazhirskaya", 55.580999, 37.659164}
    };

    const deque<Stop>& stops = guide.GetStops();
    size_t stops_size = stops.size();
    ASSERT_EQUAL(stops_size, 8u);

    for (size_t i = 0; i < stops_size; ++i) {
        ASSERT_EQUAL(stops[i].stop_name, expected_stops[i].stop_name);
        ASSERT_EQUAL(stops[i].latitude_deg, expected_stops[i].latitude_deg);
        ASSERT_EQUAL(stops[i].longitude_deg, expected_stops[i].longitude_deg);
    }

    const auto& stop_name_to_stop_ptr = guide.GetStopNameToStopPtr();
    ASSERT_EQUAL(stop_name_to_stop_ptr.size(), 8u);

    Stop* p1 = stop_name_to_stop_ptr.at("Tolstopaltsevo");
    ASSERT_EQUAL(p1->stop_name, "Tolstopaltsevo");
    ASSERT_EQUAL(p1->latitude_deg, 55.611087);
    ASSERT_EQUAL(p1->longitude_deg, 37.20829);

    Stop* p2 = stop_name_to_stop_ptr.at("Universam");
    ASSERT_EQUAL(p2->stop_name, "Universam");
    ASSERT_EQUAL(p2->latitude_deg, 55.587655);
    ASSERT_EQUAL(p2->longitude_deg, 37.645687);

    Stop* p3 = stop_name_to_stop_ptr.at("Biryulyovo Tovarnaya");
    ASSERT_EQUAL(p3->stop_name, "Biryulyovo Tovarnaya");
    ASSERT_EQUAL(p3->latitude_deg, 55.592028);
    ASSERT_EQUAL(p3->longitude_deg, 37.653656);

    Stop* p4 = stop_name_to_stop_ptr.at("Biryulyovo Passazhirskaya");
    ASSERT_EQUAL(p4->stop_name, "Biryulyovo Passazhirskaya");
    ASSERT_EQUAL(p4->latitude_deg, 55.580999);
    ASSERT_EQUAL(p4->longitude_deg, 37.659164);
}

void TestBuffer() {
    istringstream iss{R"(
10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
3
Bus 256
Bus 750
Bus 751
)"};

    TransportGuide guide;
    guide.CreateDB(iss);

    vector<string> expected_buffer = {
            "256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye",
            "750: Tolstopaltsevo - Marushkino - Rasskazovka"
    };

    // check for internal buffer containing truncated records about bus routes
    ASSERT_EQUAL(guide.GetBuffer(), expected_buffer);
}

void TestAddBusRoutes() {
    istringstream iss{R"(
10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
3
Bus 256
Bus 750
Bus 751
)"};

    TransportGuide guide;
    guide.CreateDB(iss);

    const auto& bus_routes = guide.GetBusRoutes();
    const auto& bus_name_to_busroute_ptr = guide.GetBusNameToBusRouteMapping();

    ASSERT_EQUAL(bus_routes.size(), 2u);
    ASSERT_EQUAL(bus_name_to_busroute_ptr.size(), 2u);

    set<string_view> bus_route_names_expected = {
            "256",
            "750"
    };

    set<string_view> bus_route_names;
    for(const auto& [bus_name, _] : bus_name_to_busroute_ptr) {
        bus_route_names.insert(bus_name);
    }

    ASSERT_EQUAL(bus_route_names, bus_route_names_expected);

    if (bus_name_to_busroute_ptr.count("256")) {
        auto route_256 = *bus_name_to_busroute_ptr.at("256");
        ASSERT_EQUAL(route_256[0]->stop_name, "Biryulyovo Zapadnoye");
        ASSERT_EQUAL(route_256[1]->stop_name, "Biryusinka");
        ASSERT_EQUAL(route_256[2]->stop_name, "Universam");
        ASSERT_EQUAL(route_256[3]->stop_name, "Biryulyovo Tovarnaya");
        ASSERT_EQUAL(route_256[4]->stop_name, "Biryulyovo Passazhirskaya");
        ASSERT_EQUAL(route_256[5]->stop_name, "Biryulyovo Zapadnoye");
    } else {
        cout << "Bus 256 not found" << endl;
    }

    if (bus_name_to_busroute_ptr.count("750")) {
        auto route_750 = *bus_name_to_busroute_ptr.at("750");
        ASSERT_EQUAL(route_750[0]->stop_name, "Tolstopaltsevo");
        ASSERT_EQUAL(route_750[1]->stop_name, "Marushkino");
        ASSERT_EQUAL(route_750[2]->stop_name, "Rasskazovka");
        ASSERT_EQUAL(route_750[3]->stop_name, "Marushkino");
        ASSERT_EQUAL(route_750[4]->stop_name, "Tolstopaltsevo");
    } else {
        cout << "Bus 750 not found" << endl;
    }

    ASSERT_EQUAL(5u, guide.GetUniqueStopsCountForBusRoute("256"));
    ASSERT_EQUAL(3u, guide.GetUniqueStopsCountForBusRoute("750"))
}

void TestRouteStatistics() {
    istringstream iss{R"(
10
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
3
Bus 256
Bus 750
Bus 751
)"};

    TransportGuide guide;
    guide.CreateDB(iss);

    BusRouteStats bus_256_stats_expected {
        6,
        5,
        4371.01725085208
    };

    BusRouteStats bus_256_stats = guide.GetStatsForBusRoute("256");
    ASSERT(bus_256_stats_expected == bus_256_stats);

    BusRouteStats bus_750_stats_expected {
            5,
            3,
            20939.483046751142
    };

    BusRouteStats bus_750_stats = guide.GetStatsForBusRoute("750");
    ASSERT(bus_750_stats_expected == bus_750_stats);
}

void TestStopRetrieveQuery() {
    istringstream iss_creation_part(
            R"(13
Stop Tolstopaltsevo: 55.611087, 37.20829
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
Stop Biryusinka: 55.581065, 37.64839
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
Stop Prazhskaya: 55.611678, 37.603831
)");

    istringstream iss_retrieval_part(R"(
6
Bus 256
Bus 750
Bus 751
Stop Samara
Stop Prazhskaya
Stop Biryulyovo Zapadnoye
)");

    TransportGuide guide;
    guide.CreateDB(iss_creation_part);

    istringstream iss_expected_retrieval(
            R"(Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length
Bus 751: not found
Stop Samara: not found
Stop Prazhskaya: no buses
Stop Biryulyovo Zapadnoye: buses 256 828
)");

    ostringstream oss_retrieval;
    guide.ProcessRetrievalQueries(iss_retrieval_part, oss_retrieval);
    ASSERT_EQUAL(iss_expected_retrieval.str(), oss_retrieval.str());
}


void TestAll() {
    TestRunner tr;

    RUN_TEST(tr, TestTrimFunctions);
    RUN_TEST(tr, TestSplitBy);
    RUN_TEST(tr, TestParseStopView);
    RUN_TEST(tr, TestAddStops);
    RUN_TEST(tr, TestBuffer);
    RUN_TEST(tr, TestAddBusRoutes);
    RUN_TEST(tr, TestRouteStatistics);
    RUN_TEST(tr, TestStopRetrieveQuery);
}