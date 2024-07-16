#include "requests.h"
#include "tests.h"
#include "test_runner.h"
#include <iostream>
#include <sstream>

using namespace std;

void TestJsonLoad() {
	using namespace Json;

	stringstream ss;
	ss << R"({
			"type": "Bus",
			"name": "751",
			"id": 194217464,
			"value": -100,
			"value2": 100.03,
			"value3": -1000.123456,
			"bool1": true,
			"bool2": false
		})";
	Document doc = Load(ss);
	const auto& attrs = doc.GetRoot().AsMap();
	ASSERT_EQUAL(attrs.at("type").AsString(), "Bus");
	ASSERT_EQUAL(attrs.at("name").AsString(), "751");
	ASSERT_EQUAL(attrs.at("id").AsInt(), 194217464);
	ASSERT_EQUAL(attrs.at("value").AsInt(), -100);
	ASSERT_EQUAL(attrs.at("value2").AsDouble(), 100.03);
	ASSERT_EQUAL(attrs.at("value3").AsDouble(), -1000.123456);
	ASSERT_EQUAL(attrs.at("bool1").AsBool(), true);
	ASSERT_EQUAL(attrs.at("bool2").AsBool(), false);
}

void TestPrintJson() {
	using namespace Json;
	stringstream ss;
	ss << R"({"bool1": true, "bool2": false, "id": 194217464, "name": "751", "type": "Bus", "value": -100, "value2": 100.03, "value3": -1000.12})";
	stringstream expected(ss.str());
	Document doc = Load(ss);
	stringstream os;
	os.precision(6);
	Print(doc, os);
	ASSERT_EQUAL(os.str(), expected.str());
}

void TestBusAndStopsRequests() {
	using namespace Json;
	stringstream ss;
	ss << R"({"routing_settings":{"bus_wait_time":6,"bus_velocity":40},"base_requests":[{"type":"Bus","name":"297","stops":["Biryulyovo Zapadnoye","Biryulyovo Tovarnaya","Universam","Biryulyovo Zapadnoye"],"is_roundtrip":true},{"type":"Bus","name":"635","stops":["Biryulyovo Tovarnaya","Universam","Prazhskaya"],"is_roundtrip":false},{"type":"Stop","road_distances":{"Biryulyovo Tovarnaya":2600},"longitude":37.6517,"name":"Biryulyovo Zapadnoye","latitude":55.574371},{"type":"Stop","road_distances":{"Prazhskaya":4650,"Biryulyovo Tovarnaya":1380,"Biryulyovo Zapadnoye":2500},"longitude":37.645687,"name":"Universam","latitude":55.587655},{"type":"Stop","road_distances":{"Universam":890},"longitude":37.653656,"name":"Biryulyovo Tovarnaya","latitude":55.592028},{"type":"Stop","road_distances":{},"longitude":37.603938,"name":"Prazhskaya","latitude":55.611717}],"stat_requests":[{"type":"Bus","name":"297","id":1},{"type":"Bus","name":"635","id":2},{"type":"Stop","name":"Universam","id":3},{"type":"Route","from":"Biryulyovo Zapadnoye","to":"Universam","id":4},{"type":"Route","from":"Biryulyovo Zapadnoye","to":"Prazhskaya","id":5}]})";
	Json::Document input_doc = Json::Load(ss);
	Json::Dict input_map = input_doc.GetRoot().AsMap();
	TransportCatalog db(
		Descriptions::ReadDescriptions(input_map["base_requests"].AsArray()),
		input_map["routing_settings"].AsMap(),
		input_map["render_settings"].AsMap()
	);
	const auto responses = Requests::ProcessAll(db, input_map["stat_requests"].AsArray());
	stringstream ans;
	Json::PrintValue(responses, ans);

	const string expected = R"([{"curvature": 1.42963, "request_id": 1, "route_length": 5990, "stop_count": 4, "unique_stop_count": 3}, {"curvature": 1.30156, "request_id": 2, "route_length": 11570, "stop_count": 5, "unique_stop_count": 3}, {"buses": ["297", "635"], "request_id": 3}, {"items": [{"stop_name": "Biryulyovo Zapadnoye", "time": 6, "type": "Wait"}, {"bus": "297", "span_count": 2, "time": 5.235, "type": "Bus"}], "request_id": 4, "total_time": 11.235}, {"items": [{"stop_name": "Biryulyovo Zapadnoye", "time": 6, "type": "Wait"}, {"bus": "297", "span_count": 1, "time": 3.9, "type": "Bus"}, {"stop_name": "Biryulyovo Tovarnaya", "time": 6, "type": "Wait"}, {"bus": "635", "span_count": 2, "time": 8.31, "type": "Bus"}], "request_id": 5, "total_time": 24.21}])";
	ASSERT_EQUAL(ans.str(), expected);
}

void RunAllTests() {
	TestRunner tr;
	RUN_TEST(tr, TestJsonLoad);
	RUN_TEST(tr, TestPrintJson);
	RUN_TEST(tr, TestBusAndStopsRequests);
}
