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

using namespace std;

//pair<deque<Stop>, vector<BusRoute>> ParseInput(istream& in) {
//    unordered_set<string> query_types = {
//            "Bus",
//            "Stop"
//    };
//
//    static const size_t MAX_CREAT_QUERIES = 2'000;
//    static const size_t MAX_COMMAND_LENGTH = std::max_element(begin(query_types), end(query_types),
//                                                              [](const string& lhs, const string& rhs) {
//                                                                  return lhs.size() < rhs.size();
//                                                              })->size();
//
//    // Мы не можем обрабатывать входной поток строк последовательно, т.к. нам нужно сперва сформировать deque<Stop>.
//    // Только после того, как мы обработаем всю информацию об имеющихся остановках, мы сможем формировать вектор
//    // маршрутов, содержащий указатели (shared_ptr) и string_view на эти остановки, хранимые в деке.
//    // Структура std::deque выбрана по той причине, что она не инвалидирует ссылки при добавлении в нее новых элементов.
//    // Для этого нам нужен временный буфер - в него будем складывать строки, касающиеся автобусных МАРШРУТОВ.
//    vector<string> input_buffer;
//    input_buffer.reserve(MAX_CREAT_QUERIES);
//
//    size_t add_queries_count;
//    cin >> add_queries_count;
//    deque<Stop> stops;
//
//    string line;
//    for(size_t i = 0; i < add_queries_count; ++i) {
//        getline(in, line);
//        string_view view_line{line};
//        string_view command_holder_part = string_view{line}.substr(0, MAX_COMMAND_LENGTH);
//
//        if (string command = "Stop"; command_holder_part.find(command) != string_view::npos) {
//            view_line.remove_prefix(command.size() + 1); // remove command name with space that follows it
//            stops.push_back(ParseStop(view_line));
//        } else {
//            input_buffer.push_back(std::move(line));
//        }
//    }
//}

int main() {
    TestAll();

    return 0;
}