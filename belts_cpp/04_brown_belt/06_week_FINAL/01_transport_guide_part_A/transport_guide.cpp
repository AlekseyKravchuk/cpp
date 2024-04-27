#include "transport_guide.h"

std::unordered_set<std::string> query_types = {
        "Bus",
        "Stop"
};

const size_t MAX_CREATE_QUERIES = 2'000;
const size_t MAX_COMMAND_LENGTH = std::max_element(begin(query_types), end(query_types),
                                                          [](const std::string& lhs, const std::string& rhs) {
                                                              return lhs.size() < rhs.size();
                                                          })->size();

using namespace std;

void TransportGuide::SetStops(StopsInfo&& info) {
    _stops = std::move(info.stops);
    _stop_name_to_stop_ptr = std::move(info.stop_name_to_stop_ptr);
}