#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility> // std::move
#include <vector>

#include "profile.h"

std::string MakeString() {
    return std::string(1'000'000'000, 'a');
}

std::vector<int> MakeVector() {
    return std::vector<int>(1'000'000'000, 0);
}

int main() {
    // Хотим положить во множество этот тяжелый вектор
    {
        LOG_DURATION("set::insert, for vector with variable");
        std::set<std::vector<int>> set_of_vectors;
        std::vector<int> heavy_vector = MakeVector();
        set_of_vectors.insert(heavy_vector);
    }

    {
        LOG_DURATION("set::insert, for vector WITHOUT variable");
        std::set<std::vector<int>> set_of_vectors;
        set_of_vectors.insert(MakeVector());
    }

    return 0;
}
