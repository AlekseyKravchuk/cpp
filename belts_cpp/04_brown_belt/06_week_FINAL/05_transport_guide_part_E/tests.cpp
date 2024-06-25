#include "parse.h"
#include "test_runner.h"
#include "tests.h"
#include "transport_guide.h"

#include <set>
#include <sstream>
#include <fstream>

using namespace std;

// =========================================================================
void TestAddStops() {
    string input_json_file_name = "00_inputs_and_outputs/input_1.json";
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    Json::Document document = Json::Load(input);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());

    deque<Stop> expected_stops = {
            {"Tolstopaltsevo",             55.611087, 37.20829},
            {"Marushkino",                 55.595884, 37.209755},
            {"Rasskazovka",                55.632761, 37.333324},
            {"Biryulyovo Zapadnoye",       55.574371, 37.6517},
            {"Biryusinka",                 55.581065, 37.64839},
            {"Universam",                  55.587655, 37.645687},
            {"Biryulyovo Tovarnaya",       55.592028, 37.653656},
            {"Biryulyovo Passazhirskaya",  55.580999, 37.659164},
            {"Rossoshanskaya ulitsa",      55.595579, 37.605757},
            {"Prazhskaya",                 55.611678, 37.603831}
    };

    const deque<Stop>& stops = transport_guide.GetStops();
    size_t stops_size = stops.size();
    ASSERT_EQUAL(stops_size, 10u);

    for (size_t i = 0; i < stops_size; ++i) {
        ASSERT_EQUAL(stops[i].stop_name_view, expected_stops[i].stop_name_view);
        ASSERT_EQUAL(stops[i].latitude_deg, expected_stops[i].latitude_deg);
        ASSERT_EQUAL(stops[i].longitude_deg, expected_stops[i].longitude_deg);
    }

    const auto& stop_name_to_stop_ptr = transport_guide.GetStopNameToStopPtr();
    ASSERT_EQUAL(stop_name_to_stop_ptr.size(), 10u);

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

    Stop* p10 = stop_name_to_stop_ptr.at("Prazhskaya");
    ASSERT_EQUAL(p10->stop_name_view, "Prazhskaya");
    ASSERT_EQUAL(p10->latitude_deg, 55.611678);
    ASSERT_EQUAL(p10->longitude_deg, 37.603831);
}
// =========================================================================

void TestDistancesTable() {
    string input_json_file_name = "./00_inputs_and_outputs/input_1.json";
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    Json::Document document = Json::Load(input);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());

    deque<Stop> expected_stops = {
            {"Tolstopaltsevo",             55.611087, 37.20829},
            {"Marushkino",                 55.595884, 37.209755},
            {"Rasskazovka",                55.632761, 37.333324},
            {"Biryulyovo Zapadnoye",       55.574371, 37.6517},
            {"Biryusinka",                 55.581065, 37.64839},
            {"Universam",                  55.587655, 37.645687},
            {"Biryulyovo Tovarnaya",       55.592028, 37.653656},
            {"Biryulyovo Passazhirskaya",  55.580999, 37.659164},
            {"Rossoshanskaya ulitsa",      55.595579, 37.605757},
            {"Prazhskaya",                 55.611678, 37.603831}
    };


    std::unordered_map<std::pair<CurrentStopName, NextStopName>,
                       DistanceByRoad,
                       TransportGuide::PairOfStopNameViewHasher> expected_distances_table{
            {{"Tolstopaltsevo", "Marushkino"}, 3900},
            {{"Marushkino", "Tolstopaltsevo"}, 3900},

            {{"Marushkino", "Rasskazovka"}, 9900},
            {{"Rasskazovka", "Marushkino"}, 9900},

            {{"Biryulyovo Zapadnoye", "Rossoshanskaya ulitsa"}, 7500},
            {{"Rossoshanskaya ulitsa", "Biryulyovo Zapadnoye"}, 7500},

            {{"Biryulyovo Zapadnoye", "Biryusinka"}, 1800},
            {{"Biryusinka", "Biryulyovo Zapadnoye"}, 1800},

            {{"Biryulyovo Zapadnoye", "Universam"}, 2400},
            {{"Universam", "Biryulyovo Zapadnoye"}, 2400},

            {{"Biryusinka", "Universam"}, 750},
            {{"Universam", "Biryusinka"}, 750},

            {{"Universam", "Rossoshanskaya ulitsa"}, 5600},
            {{"Rossoshanskaya ulitsa", "Universam"}, 5600},

            {{"Universam", "Biryulyovo Tovarnaya"}, 900},
            {{"Biryulyovo Tovarnaya", "Universam"}, 900},

            {{"Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya"}, 1300},
            {{"Biryulyovo Passazhirskaya", "Biryulyovo Tovarnaya"}, 1300},

            {{"Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"}, 1200},
            {{"Biryulyovo Zapadnoye", "Biryulyovo Passazhirskaya"}, 1200},
    };

    const TransportGuide::DistancesTable distances_table = transport_guide.GetDistancesTable();
    ASSERT_EQUAL(distances_table.size(), 20u);

    for (const auto& [stop_pointers_pair, distance_between_by_road] : distances_table) {
        string_view stop_name_from = stop_pointers_pair.first->stop_name_view;
        string_view stop_name_to = stop_pointers_pair.second->stop_name_view;

        auto it_expected = expected_distances_table.find({stop_name_from, stop_name_to});
        ASSERT(it_expected != expected_distances_table.end());
        ASSERT_EQUAL(it_expected->second, distance_between_by_road);
    }
}
// =========================================================================

void TestAddBusRoutes() {
    string input_json_file_name = "./00_inputs_and_outputs/input_1.json";
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    Json::Document document = Json::Load(input);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());


    const deque<TransportGuide::BusRoute>& bus_routes = transport_guide.GetBusRoutes();
    const TransportGuide::BusNameToBusRoute&  bus_name_to_bus_route_ptr = transport_guide.GetBusNameToBusRouteMapping();

    ASSERT_EQUAL(bus_routes.size(), 3u);
    ASSERT_EQUAL(bus_name_to_bus_route_ptr.size(), 3u);

    set<string_view> bus_route_names_expected = {
            "256",
            "750",
            "828"
    };

    set<string_view> bus_route_names_to_check;
    for(const auto& [bus_name, _] : bus_name_to_bus_route_ptr) {
        bus_route_names_to_check.insert(bus_name);
    }

    ASSERT_EQUAL(bus_route_names_to_check, bus_route_names_expected);

    if (bus_name_to_bus_route_ptr.count("256")) {
        auto route_256 = *bus_name_to_bus_route_ptr.at("256");
        ASSERT_EQUAL(route_256[0]->stop_name_view, "Biryulyovo Zapadnoye");
        ASSERT_EQUAL(route_256[1]->stop_name_view, "Biryusinka");
        ASSERT_EQUAL(route_256[2]->stop_name_view, "Universam");
        ASSERT_EQUAL(route_256[3]->stop_name_view, "Biryulyovo Tovarnaya");
        ASSERT_EQUAL(route_256[4]->stop_name_view, "Biryulyovo Passazhirskaya");
        ASSERT_EQUAL(route_256[5]->stop_name_view, "Biryulyovo Zapadnoye");
    } else {
        cout << "Bus 256 not found" << endl;
    }

    if (bus_name_to_bus_route_ptr.count("750")) {
        auto route_750 = *bus_name_to_bus_route_ptr.at("750");
        ASSERT_EQUAL(route_750[0]->stop_name_view, "Tolstopaltsevo");
        ASSERT_EQUAL(route_750[1]->stop_name_view, "Marushkino");
        ASSERT_EQUAL(route_750[2]->stop_name_view, "Rasskazovka");
        ASSERT_EQUAL(route_750[3]->stop_name_view, "Marushkino");
        ASSERT_EQUAL(route_750[4]->stop_name_view, "Tolstopaltsevo");
    } else {
        cout << "Bus 750 not found" << endl;
    }

    if (bus_name_to_bus_route_ptr.count("828")) {
        auto route_828 = *bus_name_to_bus_route_ptr.at("828");
        ASSERT_EQUAL(route_828[0]->stop_name_view, "Biryulyovo Zapadnoye");
        ASSERT_EQUAL(route_828[1]->stop_name_view, "Universam");
        ASSERT_EQUAL(route_828[2]->stop_name_view, "Rossoshanskaya ulitsa");
        ASSERT_EQUAL(route_828[3]->stop_name_view, "Biryulyovo Zapadnoye");
    } else {
        cout << "Bus 828 not found" << endl;
    }

    ASSERT_EQUAL(5u, transport_guide.GetUniqueStopsCountForBusRoute("256"));
    ASSERT_EQUAL(3u, transport_guide.GetUniqueStopsCountForBusRoute("750"));
    ASSERT_EQUAL(3u, transport_guide.GetUniqueStopsCountForBusRoute("828"))
}

void TestRouteStatistics() {
    string input_json_file_name = "./00_inputs_and_outputs/input_1.json";
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    Json::Document document = Json::Load(input);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());

    // ============== test bus route #256 ==============
    BusRouteStats bus_256_stats_expected {
        6,                 // stops on route
        5,                 // unique stops
        4371.01725085208,  // length by geo coordinates
        5950               // length by roads
    };

    const auto& bus_256_stats = transport_guide.GetStatsForBusRoute("256");
    if (bus_256_stats) {
        ASSERT(bus_256_stats_expected == *bus_256_stats);
    }


    // ============== test bus route #750 ==============
    BusRouteStats bus_750_stats_expected {
            5,                  // stops on route
            3,                  // unique stops
            20939.483046751142, // length by geo coordinates
            27600               // length by roads
    };

    const auto& bus_750_stats = transport_guide.GetStatsForBusRoute("750");
    ASSERT(bus_750_stats_expected == bus_750_stats);

    // ============== test bus route #828 ==============
    BusRouteStats bus_828_stats_expected {
            4,                  // stops on route
            3,                  // unique stops
            7911.8811000350706, // length by geo coordinates
            15500               // length by roads
    };

    const auto& bus_828_stats = transport_guide.GetStatsForBusRoute("828");
    ASSERT(bus_828_stats_expected == bus_828_stats);
}
// ========================

void TestProcessRetrieveQueries() {
    string input_json_file_name = "./00_inputs_and_outputs/input_1.json";;
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    Json::Document document = Json::Load(input);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());

    istringstream iss_expected_retrieval(
            R"([
  {
    "route_length": 5950,
    "request_id": 1965312327,
    "curvature": 1.36124,
    "stop_count": 6,
    "unique_stop_count": 5
  },
  {
    "route_length": 27600,
    "request_id": 519139350,
    "curvature": 1.31808,
    "stop_count": 5,
    "unique_stop_count": 3
  },
  {
    "request_id": 194217464,
    "error_message": "not found"
  },
  {
    "request_id": 746888088,
    "error_message": "not found"
  },
  {
    "buses": [],
    "request_id": 65100610
  },
  {
    "buses": [
      "256",
      "828"
    ],
    "request_id": 1042838872
  }
])");

    ostringstream oss_retrieval;
    transport_guide.ProcessRetrieveQueries(root_map.at("stat_requests").AsArray(), oss_retrieval);

    // дублируем вывод в файл в целях отладки
    string output_file_name = "output.json";
    ofstream output(output_file_name);

    if (!output) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened for write operations.");
    }
    transport_guide.ProcessRetrieveQueries(root_map.at("stat_requests").AsArray(), output);
    // ==========================

    ASSERT_EQUAL(iss_expected_retrieval.str(), oss_retrieval.str());
}
// ========================

void TestAll() {
    TestRunner tr;

    /*1*/  RUN_TEST(tr, TestAddStops);
    /*2*/  RUN_TEST(tr, TestDistancesTable);
    /*3*/  RUN_TEST(tr, TestAddBusRoutes);
    /*4*/  RUN_TEST(tr, TestRouteStatistics);
    /*5*/  RUN_TEST(tr, TestProcessRetrieveQueries);
}