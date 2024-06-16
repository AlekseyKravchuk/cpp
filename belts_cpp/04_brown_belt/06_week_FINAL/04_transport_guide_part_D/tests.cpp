#include "parse.h"
#include "test_runner.h"
#include "tests.h"
#include "transport_guide.h"

#include <set>
#include <sstream>
#include <fstream>

using namespace std;

void TestAddStops() {
    string input_json_file_name = "input.json";
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    Json::Document document = Json::Load(input);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());

    deque<Stop> expected_stops = {
            {"Tolstopaltsevo", 55.611087, 37.20829},
            {"Marushkino", 55.595884, 37.209755},
            {"Rasskazovka", 55.632761, 37.333324},
            {"Biryulyovo Zapadnoye", 55.574371, 37.6517},
            {"Biryusinka", 55.581065, 37.64839},
            {"Universam", 55.587655, 37.645687},
            {"Biryulyovo Tovarnaya", 55.592028, 37.653656},
            {"Biryulyovo Passazhirskaya", 55.580999, 37.659164},
    };

    const deque<Stop>& stops = transport_guide.GetStops();
    size_t stops_size = stops.size();
    ASSERT_EQUAL(stops_size, 8u);

    for (size_t i = 0; i < stops_size; ++i) {
        ASSERT_EQUAL(stops[i].stop_name_view, expected_stops[i].stop_name_view);
        ASSERT_EQUAL(stops[i].latitude_deg, expected_stops[i].latitude_deg);
        ASSERT_EQUAL(stops[i].longitude_deg, expected_stops[i].longitude_deg);
    }

    const auto& stop_name_to_stop_ptr = transport_guide.GetStopNameToStopPtr();
    ASSERT_EQUAL(stop_name_to_stop_ptr.size(), 8u);

    Stop* p1 = stop_name_to_stop_ptr.at("Tolstopaltsevo");
    ASSERT_EQUAL(p1->stop_name_view, "Tolstopaltsevo");
    ASSERT_EQUAL(p1->latitude_deg, 55.611087);
    ASSERT_EQUAL(p1->longitude_deg, 37.20829);

    Stop* p2 = stop_name_to_stop_ptr.at("Universam");
    ASSERT_EQUAL(p2->stop_name_view, "Universam");
    ASSERT_EQUAL(p2->latitude_deg, 55.587655);
    ASSERT_EQUAL(p2->longitude_deg, 37.645687);

    Stop* p3 = stop_name_to_stop_ptr.at("Biryulyovo Tovarnaya");
    ASSERT_EQUAL(p3->stop_name_view, "Biryulyovo Tovarnaya");
    ASSERT_EQUAL(p3->latitude_deg, 55.592028);
    ASSERT_EQUAL(p3->longitude_deg, 37.653656);

    Stop* p4 = stop_name_to_stop_ptr.at("Biryulyovo Passazhirskaya");
    ASSERT_EQUAL(p4->stop_name_view, "Biryulyovo Passazhirskaya");
    ASSERT_EQUAL(p4->latitude_deg, 55.580999);
    ASSERT_EQUAL(p4->longitude_deg, 37.659164);
}

//void TestAddStops_Extended() {
//    istringstream iss{R"(
//13
//Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
//Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
//Stop Biryusinka: 55.581065, 37.64839, 750m to Universam
//Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye
//Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
//Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
//Stop Prazhskaya: 55.611678, 37.603831
//6
//Bus 256
//Bus 750
//Bus 751
//Stop Samara
//Stop Prazhskaya
//Stop Biryulyovo Zapadnoye
//)"};
//
//    TransportGuide guide;
//    guide.ProcessCreationQueries(iss);
//
//    deque<Stop> expected_stops = {
//            {"Tolstopaltsevo", 55.611087, 37.20829},
//            {"Marushkino", 55.595884, 37.209755},
//            {"Rasskazovka", 55.632761, 37.333324},
//            {"Biryulyovo Zapadnoye", 55.574371, 37.6517},
//            {"Biryusinka", 55.581065, 37.64839},
//            {"Universam", 55.587655, 37.645687},
//            {"Biryulyovo Tovarnaya", 55.592028, 37.653656},
//            {"Biryulyovo Passazhirskaya", 55.580999, 37.659164},
//            {"Rossoshanskaya ulitsa", 55.595579, 37.605757},
//            {"Prazhskaya", 55.611678, 37.603831}
//    };
//
//    const deque<Stop>& stops = guide.GetStops();
//    size_t stops_size = stops.size();
//    ASSERT_EQUAL(stops_size, 10u);
//
//    for (size_t i = 0; i < stops_size; ++i) {
//        ASSERT_EQUAL(stops[i].stop_name_view, expected_stops[i].stop_name_view);
//        ASSERT_EQUAL(stops[i].latitude_deg, expected_stops[i].latitude_deg);
//        ASSERT_EQUAL(stops[i].longitude_deg, expected_stops[i].longitude_deg);
//    }
//
//    const auto& stop_name_to_stop_ptr = guide.GetStopNameToStopPtr();
//    ASSERT_EQUAL(stop_name_to_stop_ptr.size(), 10u);
//
//    Stop* p1 = stop_name_to_stop_ptr.at("Tolstopaltsevo");
//    ASSERT_EQUAL(p1->stop_name_view, "Tolstopaltsevo");
//    ASSERT_EQUAL(p1->latitude_deg, 55.611087);
//    ASSERT_EQUAL(p1->longitude_deg, 37.20829);
//
//    Stop* p2 = stop_name_to_stop_ptr.at("Universam");
//    ASSERT_EQUAL(p2->stop_name_view, "Universam");
//    ASSERT_EQUAL(p2->latitude_deg, 55.587655);
//    ASSERT_EQUAL(p2->longitude_deg, 37.645687);
//
//    Stop* p3 = stop_name_to_stop_ptr.at("Biryulyovo Tovarnaya");
//    ASSERT_EQUAL(p3->stop_name_view, "Biryulyovo Tovarnaya");
//    ASSERT_EQUAL(p3->latitude_deg, 55.592028);
//    ASSERT_EQUAL(p3->longitude_deg, 37.653656);
//
//    Stop* p4 = stop_name_to_stop_ptr.at("Biryulyovo Passazhirskaya");
//    ASSERT_EQUAL(p4->stop_name_view, "Biryulyovo Passazhirskaya");
//    ASSERT_EQUAL(p4->latitude_deg, 55.580999);
//    ASSERT_EQUAL(p4->longitude_deg, 37.659164);
//}

// ====================================================
//void TestDistancesTable() {
//    istringstream iss{R"(
//13
//Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
//Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
//Stop Biryusinka: 55.581065, 37.64839, 750m to Universam
//Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye
//Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
//Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
//Stop Prazhskaya: 55.611678, 37.603831
//6
//Bus 256
//Bus 750
//Bus 751
//Stop Samara
//Stop Prazhskaya
//Stop Biryulyovo Zapadnoye
//)"};
//
//    TransportGuide guide;
//    guide.ProcessCreationQueries(iss);
//    guide.CreateDistancesTable();
//
//    std::unordered_map<std::pair<CurrentStopName, NextStopName>,
//                       DistanceByRoad,
//                       TransportGuide::PairOfStopNameViewHasher> expected_distances_table{
//            {{"Tolstopaltsevo", "Marushkino"}, 3900},
//            {{"Marushkino", "Tolstopaltsevo"}, 3900},
//
//            {{"Marushkino", "Rasskazovka"}, 9900},
//            {{"Rasskazovka", "Marushkino"}, 9900},
//
//            {{"Biryulyovo Zapadnoye", "Rossoshanskaya ulitsa"}, 7500},
//            {{"Rossoshanskaya ulitsa", "Biryulyovo Zapadnoye"}, 7500},
//
//            {{"Biryulyovo Zapadnoye", "Biryusinka"}, 1800},
//            {{"Biryusinka", "Biryulyovo Zapadnoye"}, 1800},
//
//            {{"Biryulyovo Zapadnoye", "Universam"}, 2400},
//            {{"Universam", "Biryulyovo Zapadnoye"}, 2400},
//
//            {{"Biryusinka", "Universam"}, 750},
//            {{"Universam", "Biryusinka"}, 750},
//
//            {{"Universam", "Rossoshanskaya ulitsa"}, 5600},
//            {{"Rossoshanskaya ulitsa", "Universam"}, 5600},
//
//            {{"Universam", "Biryulyovo Tovarnaya"}, 900},
//            {{"Biryulyovo Tovarnaya", "Universam"}, 900},
//
//            {{"Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya"}, 1300},
//            {{"Biryulyovo Passazhirskaya", "Biryulyovo Tovarnaya"}, 1300},
//
//            {{"Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"}, 1200},
//            {{"Biryulyovo Zapadnoye", "Biryulyovo Passazhirskaya"}, 1200},
//    };
//
//    const TransportGuide::DistancesTable distances_table = guide.GetDistancesTable();
//    ASSERT_EQUAL(distances_table.size(), 20u);
//
//    for (const auto& [stop_pointers_pair, distance_between_by_road] : distances_table) {
//        string_view stop_name_from = stop_pointers_pair.first->stop_name_view;
//        string_view stop_name_to = stop_pointers_pair.second->stop_name_view;
//
//        auto it_expected = expected_distances_table.find({stop_name_from, stop_name_to});
//        ASSERT(it_expected != expected_distances_table.end());
//        ASSERT_EQUAL(it_expected->second, distance_between_by_road);
//    }
//}
//
//void TestBuffer() {
//    istringstream iss{R"(
//10
//Stop Tolstopaltsevo: 55.611087, 37.20829
//Stop Marushkino: 55.595884, 37.209755
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
//Stop Biryusinka: 55.581065, 37.64839
//Stop Universam: 55.587655, 37.645687
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
//3
//Bus 256
//Bus 750
//Bus 751
//)"};
//
//    TransportGuide guide;
//    guide.ProcessCreationQueries(iss);
//
//    vector<string> expected_buffer = {
//            "256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye",
//            "750: Tolstopaltsevo - Marushkino - Rasskazovka"
//    };
//}
//
//void TestAddBusRoutes() {
//    istringstream iss{R"(
//10
//Stop Tolstopaltsevo: 55.611087, 37.20829
//Stop Marushkino: 55.595884, 37.209755
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
//Stop Biryusinka: 55.581065, 37.64839
//Stop Universam: 55.587655, 37.645687
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
//3
//Bus 256
//Bus 750
//Bus 751
//)"};
//
//    TransportGuide guide;
//    guide.ProcessCreationQueries(iss);
//
//    const auto& bus_routes = guide.GetBusRoutes();
//    const auto& bus_name_to_busroute_ptr = guide.GetBusNameToBusRouteMapping();
//
//    ASSERT_EQUAL(bus_routes.size(), 2u);
//    ASSERT_EQUAL(bus_name_to_busroute_ptr.size(), 2u);
//
//    set<string_view> bus_route_names_expected = {
//            "256",
//            "750"
//    };
//
//    set<string_view> bus_route_names;
//    for(const auto& [bus_name, _] : bus_name_to_busroute_ptr) {
//        bus_route_names.insert(bus_name);
//    }
//
//    ASSERT_EQUAL(bus_route_names, bus_route_names_expected);
//
//    if (bus_name_to_busroute_ptr.count("256")) {
//        auto route_256 = *bus_name_to_busroute_ptr.at("256");
//        ASSERT_EQUAL(route_256[0]->stop_name_view, "Biryulyovo Zapadnoye");
//        ASSERT_EQUAL(route_256[1]->stop_name_view, "Biryusinka");
//        ASSERT_EQUAL(route_256[2]->stop_name_view, "Universam");
//        ASSERT_EQUAL(route_256[3]->stop_name_view, "Biryulyovo Tovarnaya");
//        ASSERT_EQUAL(route_256[4]->stop_name_view, "Biryulyovo Passazhirskaya");
//        ASSERT_EQUAL(route_256[5]->stop_name_view, "Biryulyovo Zapadnoye");
//    } else {
//        cout << "Bus 256 not found" << endl;
//    }
//
//    if (bus_name_to_busroute_ptr.count("750")) {
//        auto route_750 = *bus_name_to_busroute_ptr.at("750");
//        ASSERT_EQUAL(route_750[0]->stop_name_view, "Tolstopaltsevo");
//        ASSERT_EQUAL(route_750[1]->stop_name_view, "Marushkino");
//        ASSERT_EQUAL(route_750[2]->stop_name_view, "Rasskazovka");
//        ASSERT_EQUAL(route_750[3]->stop_name_view, "Marushkino");
//        ASSERT_EQUAL(route_750[4]->stop_name_view, "Tolstopaltsevo");
//    } else {
//        cout << "Bus 750 not found" << endl;
//    }
//
//    ASSERT_EQUAL(5u, guide.GetUniqueStopsCountForBusRoute("256"));
//    ASSERT_EQUAL(3u, guide.GetUniqueStopsCountForBusRoute("750"))
//}
//
//void TestRouteStatistics() {
//    istringstream iss{R"(
//10
//Stop Tolstopaltsevo: 55.611087, 37.20829
//Stop Marushkino: 55.595884, 37.209755
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517
//Stop Biryusinka: 55.581065, 37.64839
//Stop Universam: 55.587655, 37.645687
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
//3
//Bus 256
//Bus 750
//Bus 751
//)"};
//
//    TransportGuide guide;
//    guide.ProcessCreationQueries(iss);
//
//    BusRouteStats bus_256_stats_expected {
//        6,
//        5,
//        {4371.01725085208, 0}
//    };
//
//    BusRouteStats bus_256_stats = guide.GetStatsForBusRoute("256");
//    ASSERT(bus_256_stats_expected == bus_256_stats);
//
//    BusRouteStats bus_750_stats_expected {
//            5,
//            3,
//            {20939.483046751142, 0}
//    };
//
//    BusRouteStats bus_750_stats = guide.GetStatsForBusRoute("750");
//    ASSERT(bus_750_stats_expected == bus_750_stats);
//}
//
//void TestStopRetrieveQuery() {
//    istringstream iss_creation_part(
//            R"(13
//Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino
//Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka
//Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
//Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
//Stop Rasskazovka: 55.632761, 37.333324
//Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
//Stop Biryusinka: 55.581065, 37.64839, 750m to Universam
//Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya
//Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya
//Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye
//Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye
//Stop Rossoshanskaya ulitsa: 55.595579, 37.605757
//Stop Prazhskaya: 55.611678, 37.603831
//)");
//
//    istringstream iss_retrieval_part(R"(
//6
//Bus 256
//Bus 750
//Bus 751
//Stop Samara
//Stop Prazhskaya
//Stop Biryulyovo Zapadnoye
//)");
//
//    TransportGuide guide;
//    guide.ProcessCreationQueries(iss_creation_part);
//
//    istringstream iss_expected_retrieval(
//            R"(Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.361239 curvature
//Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature
//Bus 751: not found
//Stop Samara: not found
//Stop Prazhskaya: no buses
//Stop Biryulyovo Zapadnoye: buses 256 828
//)");
//
//    ostringstream oss_retrieval;
//    guide.ProcessRetrievalQueries(iss_retrieval_part, oss_retrieval);
//    ASSERT_EQUAL(iss_expected_retrieval.str(), oss_retrieval.str());
//}


void TestAll() {
    TestRunner tr;

    /*1*/  RUN_TEST(tr, TestAddStops);
//    /*2*/  RUN_TEST(tr, TestAddStops_Extended);
//    /*3*/  RUN_TEST(tr, TestBuffer);
//    /*4*/  RUN_TEST(tr, TestAddBusRoutes);
//    /*5*/  RUN_TEST(tr, TestRouteStatistics);
//    /*6*/  RUN_TEST(tr, TestStopRetrieveQuery);
//    /*7*/ RUN_TEST(tr, TestDistancesTable);
}