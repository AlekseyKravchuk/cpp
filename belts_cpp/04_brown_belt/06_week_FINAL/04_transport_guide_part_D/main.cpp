#include "json.h"
#include "tests.h"
#include "transport_guide.h"

using namespace std;

int main() {
//    TestAll();

    Json::Document document = Json::Load(cin);
    const map<string, Json::Node>& root_map = document.GetRoot().AsMap();

    TransportGuide transport_guide;
    transport_guide.CreateDataBaseFromJSON(root_map.at("base_requests").AsArray());
    transport_guide.ProcessRetrieveQueries(root_map.at("stat_requests").AsArray());

    return 0;
}