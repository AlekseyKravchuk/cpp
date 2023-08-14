#include "server_stats.h"

Stats ServeRequests(std::istream& input) {
    Stats result;

    // проблема в том, что время жизни переменной "line" ограничено циклом "for"
    for (std::string line; std::getline(input, line);) {
        const HttpRequest req = ParseRequest(line);
        result.AddUri(req.uri);
        result.AddMethod(req.method);
    }
    
    return result;
}
