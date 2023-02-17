#include <iostream>
#include <map>
#include <utility>

using namespace std::literals;

template <typename T, typename U>
void PrintMap(const std::map<T, U>& m) {
    std::cout << "map size = " << m.size() << std::endl;
    for (const auto& [key, value] : m) {
        std::cout << key << ": " << value << std::endl;
    }
}

template <typename T, typename U>
auto BuildReverseMap(const std::map<T, U>& dict) {
    std::map<U, T> reverted_dict;
    for (const auto& [key, value] : dict) {
        reverted_dict[value] = key;
    }

    return reverted_dict;
}

int main() {
    std::map<int, std::string> events;
    events[1950] = "Bjarne Stroustrup's birth"s;
    events[1941] = "Dennis Ritchie's birth"s;
    events[1970] = "UNIX epoch start"s;
    PrintMap(events);

    std::cout << events[1950] << std::endl;
    events.erase(1970);
    PrintMap(events);

    std::map<std::string, int> reverted_dict = BuildReverseMap(events);
    PrintMap(reverted_dict);

    auto reconstructed_events = BuildReverseMap(reverted_dict);
    PrintMap(reconstructed_events);

    return 0;
}
