#include "stats.h"

std::set<std::string> SUPPORTED_METHODS = {"GET"s, "POST"s, "PUT"s, "DELETE"s};
std::set<std::string> SUPPORTED_URIs = {"/"s, "/order"s, "/product"s, "/basket"s, "/help"s};
std::set<std::string> SUPPORTED_PROTOCOLS = {"HTTP/1.1"s};

std::string UNKNOWN_METHOD = "UNKNOWN"s;
std::string UNKNOWN_URI = "/unknown"s;
std::string UNKNOWN_PROTOCOL = "/unknown_protocol"s;

void Stats::AddMethod(std::string_view method) {
    ++_method_stats[FindMethodFromSupported(method)];
}

void Stats::AddUri(std::string_view uri) {
    ++_uri_stats[FindUriFromSupported(uri)];
}

const std::map<std::string_view, int>& Stats::GetMethodStats() const {
    return _method_stats;
}

const std::map<std::string_view, int>& Stats::GetUriStats() const {
    return _uri_stats;
}

HttpRequest ParseRequest(std::string_view line) {
    HttpRequest http_request;
    size_t space_pos = line.find(' ');

    // между словами может быть несколько пробелов
    auto remove_extra_spaces = [&line, &space_pos]() {
        while (line[space_pos] == ' ') {
            ++space_pos;
        }
    };

    http_request.method = FindMethodFromSupported(std::string(line.substr(0, space_pos)));
    remove_extra_spaces();
    line.remove_prefix(space_pos);

    space_pos = line.find(' ');
    http_request.uri = FindUriFromSupported(std::string(line.substr(0, space_pos)));
    remove_extra_spaces();
    line.remove_prefix(space_pos);

    space_pos = line.find(' ');
    http_request.uri = FindProtocolFromSupported(std::string(line.substr(0, space_pos)));
    remove_extra_spaces();
    line.remove_prefix(space_pos);

    return http_request;
}

std::string_view FindMethodFromSupported(std::string_view method_name) {
    return (SUPPORTED_METHODS.count(std::string(method_name)))
               ? *SUPPORTED_METHODS.find(std::string(method_name))
               : UNKNOWN_METHOD;
}

std::string_view FindUriFromSupported(std::string_view uri) {
    return (SUPPORTED_URIs.count(std::string(uri)))
               ? *SUPPORTED_URIs.find(std::string(uri))
               : UNKNOWN_URI;
}

std::string_view FindProtocolFromSupported(std::string_view protocol) {
    return (SUPPORTED_PROTOCOLS.count(std::string(protocol)))
               ? *SUPPORTED_PROTOCOLS.find(std::string(protocol))
               : UNKNOWN_PROTOCOL;
}