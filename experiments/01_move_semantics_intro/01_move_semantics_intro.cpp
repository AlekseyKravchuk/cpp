#include <iostream>
#include <string>
#include <vector>

#include "profile.h"

std::string MakeString() {
    return std::string(1'000'000'000, 'a');
}

int main() {
    // Generate string USING variable and push it into vector: 865 ms
    // Generate string WITHOUT variable and push it into vector: 401 ms
    // Generate string using string CTOR and push it into vector: 401 ms

    std::vector<std::string> strings;

    {
        LOG_DURATION("Generate string USING variable and push it into vector");
        std::string heavy_string = MakeString();
        strings.push_back(heavy_string);
    }

    {
        LOG_DURATION("Generate string WITHOUT variable and push it into vector");
        strings.push_back(MakeString());
    }

    {
        LOG_DURATION("Generate string using string CTOR and push it into vector");
        strings.push_back(std::string(1'000'000'000, 'a'));
    }

    return 0;
}
