#include "tests.h"

void TestParsing() {
    const std::string input =
        R"(GET / HTTP/1.1
    POST /order HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    GET /order HTTP/1.1
    PUT /product HTTP/1.1
    GET /basket HTTP/1.1
    DELETE /product HTTP/1.1
    GET / HTTP/1.1
    GET / HTTP/1.1
    GET /help HTTP/1.1
    GET /upyachka HTTP/1.1
    GET /unexpected HTTP/1.1
    HEAD / HTTP/1.1)";

    std::stringstream iss(input);
    std::vector<std::string> lines;
    const std::string whitespaces_list = " "s;

    for (std::string line; std::getline(iss, line);) {
        // trim leading whitespaces
        size_t pos = line.find_first_not_of(whitespaces_list);
        if (pos != line.npos) {
            line.erase(0, pos);
        }

        // trim trailing whitespaces
        pos = line.find_last_not_of(whitespaces_list);  // ищем первый непробельный символ, начиная с конца строки
        ++pos;                                          // сдвигаемся на одну позицию вправо, т.е. на первый пробел после слова
        line.erase(pos);

        lines.push_back(line);
    }

    std::vector<HttpRequest> exptected_requests = {
        {"GET", "/", "HTTP/1.1"},
        {"POST", "/order", "HTTP/1.1"},
        {"POST", "/product", "HTTP/1.1"},
        {"POST", "/product", "HTTP/1.1"},
        {"POST", "/product", "HTTP/1.1"},
        {"GET", "/order", "HTTP/1.1"},
        {"PUT", "/product", "HTTP/1.1"},
        {"GET", "/basket", "HTTP/1.1"},
        {"DELETE", "/product", "HTTP/1.1"},
        {"GET", "/", "HTTP/1.1"},
        {"GET", "/", "HTTP/1.1"},
        {"GET", "/help", "HTTP/1.1"},
        {"GET", "/upyachka", "HTTP/1.1"},
        {"GET", "/unexpected", "HTTP/1.1"},
        {"HEAD", "/", "HTTP/1.1"}};

    std::vector<HttpRequest> parsed_requests;
    for (std::string_view line_view : lines) {
        const HttpRequest req = ParseRequest(line_view);
        parsed_requests.push_back(req);
    }

    ASSERT_EQUAL(parsed_requests.size(), exptected_requests.size());
    for (size_t i = 0; i < parsed_requests.size(); ++i) {
        ASSERT_EQUAL(parsed_requests[i].method, exptected_requests[i].method);
        ASSERT_EQUAL(parsed_requests[i].uri, exptected_requests[i].uri);
        ASSERT_EQUAL(parsed_requests[i].protocol, exptected_requests[i].protocol);
    }
}

void TestBasic() {
    const std::string input =
        R"(GET / HTTP/1.1
    POST /order HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    GET /order HTTP/1.1
    PUT /product HTTP/1.1
    GET /basket HTTP/1.1
    DELETE /product HTTP/1.1
    GET / HTTP/1.1
    GET / HTTP/1.1
    GET /help HTTP/1.1
    GET /upyachka HTTP/1.1
    GET /unexpected HTTP/1.1
    HEAD / HTTP/1.1)";

    const std::map<std::string_view, int> expected_method_count = {
        {"GET", 8},
        {"PUT", 1},
        {"POST", 4},
        {"DELETE", 1},
        {"UNKNOWN", 1},
    };

    const std::map<std::string_view, int> expected_url_count = {
        {"/", 4},
        {"/order", 2},
        {"/product", 5},
        {"/basket", 1},
        {"/help", 1},
        {"unknown", 2},
    };

    std::istringstream is(input);
    const Stats stats = ServeRequests(is);

    ASSERT_EQUAL(stats.GetMethodStats(), expected_method_count);
    ASSERT_EQUAL(stats.GetUriStats(), expected_url_count);
}

void TestAbsentParts() {
    // Методы GetMethodStats и GetUriStats должны возвращать словари
    // с полным набором ключей, даже если какой-то из них не встречался

    const std::map<std::string_view, int> expected_method_count = {
        {"GET", 0},
        {"PUT", 0},
        {"POST", 0},
        {"DELETE", 0},
        {"UNKNOWN", 0},
    };
    const std::map<std::string_view, int> expected_url_count = {
        {"/", 0},
        {"/order", 0},
        {"/product", 0},
        {"/basket", 0},
        {"/help", 0},
        {"unknown", 0},
    };
    const Stats default_constructed;

    ASSERT_EQUAL(default_constructed.GetMethodStats(), expected_method_count);
    ASSERT_EQUAL(default_constructed.GetUriStats(), expected_url_count);
}