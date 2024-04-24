#include "parse.h"
#include "tests.h"
#include "transport_guide.h"

#include <utility>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <algorithm>  // std::max_element
#include <memory>

using namespace std;

unordered_set<string> query_types = {
        "Bus",
        "Stop"
};

static const size_t MAX_CREATE_QUERIES = 2'000;
static const size_t MAX_COMMAND_LENGTH = std::max_element(begin(query_types), end(query_types),
                                                          [](const string& lhs, const string& rhs) {
                                                              return lhs.size() < rhs.size();
                                                          })->size();

pair<deque<Stop>, vector<BusRoute>> ParseInput(istream& in) {
    // Мы не можем обрабатывать входной поток строк последовательно, т.к. нам нужно сперва сформировать deque<Stop>.
    // Только после того, как мы обработаем всю информацию об имеющихся остановках, мы сможем формировать вектор
    // маршрутов, содержащий указатели (shared_ptr) и string_view на эти остановки, хранимые в деке.
    // Структура std::deque выбрана по той причине, что она не инвалидирует ссылки при добавлении в нее новых элементов.
    // Для этого нам нужен временный буфер - в него будем складывать строки, касающиеся автобусных МАРШРУТОВ.
    vector<string> input_buffer;
    input_buffer.reserve(MAX_CREATE_QUERIES);

    size_t add_queries_count;
    cin >> add_queries_count;
    deque<Stop> stops;
    unordered_map<string_view, shared_ptr<Stop>> stop_name_to_stop_ptr;

    // обрабатываем ту часть входного потока, которая касается остановок
    string line;
    for(size_t i = 0; i < add_queries_count; ++i) {
        getline(in, line);
        string_view s{line};
        string_view command_holder_part = s.substr(0, MAX_COMMAND_LENGTH);

        if (string command = "Stop"; command_holder_part.find(command) != string_view::npos) {
            s.remove_prefix(command.size() + 1); // remove command name with space that follows it
            stops.push_back(ParseStopView(s));
            // TODO: fill in stop_name_to_stop_ptr using stops.back()
        } else {
            input_buffer.push_back(std::move(line));
        }
    }

    // теперь у нас есть полная информация об остановках и можем приступать к формированию маршрутов,
    // которые состоят из этих остановок

    // TODO: обработать информацию о маршрутах, которые хранятся в "input_buffer"

    // TODO: place correct return value
    return {};  // STUB!!!
}

int main() {
    TestAll();

    return 0;
}