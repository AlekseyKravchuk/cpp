#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>

#include "json.h"

using namespace std;

int main() {
    string json_filename = "/home/kav/cpp_code/belts_cpp/04_brown_belt/06_week_FINAL/04_test_JSON_parser/input.json";
    std::ifstream in{json_filename};

    if (!in) {
        throw std::logic_error(R"(File: ")" + json_filename + R"(" is not opened.)");
    }

    Json::Document document = Json::Load(in);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();
    vector<Json::Node> base_requests = root_map.at("base_requests").AsArray();
    vector<Json::Node> stat_requests = root_map.at("stat_requests").AsArray();

    cout << "Ok" << endl;

    return 0;
}
