#include "descriptions.h"
#include "json.h"
#include "requests.h"
#include "sphere.h"
#include "transport_catalog.h"
#include "utils.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {

    string input_json_file_name = "00_inputs_and_outputs/input_1.json";
    ifstream input(input_json_file_name);

    if (!input) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

//    const auto input_doc = Json::Load(cin);
    const auto input_doc = Json::Load(input);
    const auto& input_map = input_doc.GetRoot().AsMap();

    const TransportCatalog db(Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
                              input_map.at("routing_settings").AsMap());

    string output_file_name = "output.json";
    ofstream output(output_file_name);

    if (!output) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened for write operations.");
    }

//    Json::PrintValue(Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()), cout);
    Json::PrintValue(Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()), output);
    cout << endl;

    return 0;
}
