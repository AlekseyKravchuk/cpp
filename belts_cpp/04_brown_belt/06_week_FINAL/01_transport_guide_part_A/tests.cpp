#include "parse.h"
#include "test_runner.h"
#include "tests.h"

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
                "       Tolstopaltsevo:  55.611087,   37.20829                     ";
        Stop expected_stop = Stop{
                "Tolstopaltsevo",
                55.611087,
                37.20829};

        Stop result = ParseStopView(s);
        ASSERT_EQUAL(expected_stop.stop_name, result.stop_name);
        ASSERT_EQUAL(expected_stop.latitude, result.latitude);
        ASSERT_EQUAL(expected_stop.longitude, result.longitude);
    }
}

void TestParseStopQueries() {
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
    vector<string> result_buffer;
    result_buffer.reserve(MAX_CREATE_QUERIES);

    vector<string> expected_buffer = {
            "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye",
            "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka"
    };

    auto [stops, stop_name_to_stop_ptr] = ParseStopQueries(iss, result_buffer);
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

    ASSERT_EQUAL(stops.size(), 8u);

    for (size_t i = 0; i < stops.size(); ++i) {
        ASSERT_EQUAL(stops[i].stop_name, expected_stops[i].stop_name);
        ASSERT_EQUAL(stops[i].latitude, expected_stops[i].latitude);
        ASSERT_EQUAL(stops[i].longitude, expected_stops[i].longitude);
    }

    ASSERT_EQUAL(stop_name_to_stop_ptr.size(), 8u);

    Stop* p1 = stop_name_to_stop_ptr["Tolstopaltsevo"];
    ASSERT_EQUAL(p1->stop_name, "Tolstopaltsevo");
    ASSERT_EQUAL(p1->latitude, 55.611087);
    ASSERT_EQUAL(p1->longitude, 37.20829);

    Stop* p2 = stop_name_to_stop_ptr["Universam"];
    ASSERT_EQUAL(p2->stop_name, "Universam");
    ASSERT_EQUAL(p2->latitude, 55.587655);
    ASSERT_EQUAL(p2->longitude, 37.645687);

    Stop* p3 = stop_name_to_stop_ptr["Biryulyovo Tovarnaya"];
    ASSERT_EQUAL(p3->stop_name, "Biryulyovo Tovarnaya");
    ASSERT_EQUAL(p3->latitude, 55.592028);
    ASSERT_EQUAL(p3->longitude, 37.653656);

    Stop* p4 = stop_name_to_stop_ptr["Biryulyovo Passazhirskaya"];
    ASSERT_EQUAL(p4->stop_name, "Biryulyovo Passazhirskaya");
    ASSERT_EQUAL(p4->latitude, 55.580999);
    ASSERT_EQUAL(p4->longitude, 37.659164);
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestTrimFunctions);
    RUN_TEST(tr, TestSplitBy);
    RUN_TEST(tr, TestParseStopView);
    RUN_TEST(tr, TestParseStopQueries);
}