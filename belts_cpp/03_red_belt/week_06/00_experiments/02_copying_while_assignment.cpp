#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../../log_duration.h"

// функция возвращает "тяжелую" строку
std::string MakeString() {
    return std::string(1'000'000'000, 'a');
}

std::vector<int> MakeVector() {
    return std::vector<int>(100'000'000, 0);
}

int main() {
    std::string target_string = "old value";

    {
        LOG_DURATION("assigment, with temp variable");
        std::string target_string = "old value";
        std::string source_string = MakeString();
        target_string = source_string;
    }

    {
        LOG_DURATION("assigment, WITHOUT temp variable");
        std::string target_string = "old value";
        target_string = MakeString();
    }

    {
        LOG_DURATION("set::insert with temp variable");
        std::set<std::string> strings;
        std::string heavy_string = MakeString();
        strings.insert(heavy_string);
    }

    {
        LOG_DURATION("set::insert WITHOUT temp variable");
        std::set<std::string> strings;
        strings.insert(MakeString());
    }

    {
        LOG_DURATION("map::operator[], with temp variables");
        std::map<std::string, std::string> strings;
        std::string key = MakeString();
        std::string value = MakeString();
        strings[key] = value;
    }

    {
        LOG_DURATION("map::operator[], std::move for map value");
        std::map<std::string, std::string> strings;
        std::string key = MakeString();
        strings[key] = MakeString();
    }

    {
        LOG_DURATION("map::operator[], std::move for both key and value of map");
        std::map<std::string, std::string> strings;
        strings[MakeString()] = MakeString();
    }

    {
        LOG_DURATION("set::insert for vector, with variable");
        std::set<std::vector<int>> vectors;
        std::vector<int> heavy_vector = MakeVector();
        vectors.insert(heavy_vector);
    }

    {
        LOG_DURATION("set::insert for vector, WITHOUT variable");
        std::set<std::vector<int>> vectors;
        vectors.insert(MakeVector());
    }

    return 0;
}
