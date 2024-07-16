#include "requests.h"
//#include "tests.h"

using namespace std;

int main() {
	//RunAllTests();

	cout.precision(6);

	try {
		Json::Document input_doc = Json::Load(cin);
		Json::Dict input_map = input_doc.GetRoot().AsMap();
		TransportCatalog db(
			Descriptions::ReadDescriptions(input_map["base_requests"].AsArray()),
			input_map["routing_settings"].AsMap(),
			input_map["render_settings"].AsMap()
		);
		const auto responses = Requests::ProcessAll(db, input_map["stat_requests"].AsArray());
		Json::PrintValue(responses, cout);
	}
	catch (const runtime_error & e) {
		cerr << "Exception: " << e.what() << '\n';
	}

	return 0;
}
