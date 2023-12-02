#include <algorithm>
#include <iostream>
#include <queue>
#include <utility> // std::pair
#include <vector>

#include "../../print.h"

int main() {
    std::vector<std::pair<size_t, size_t>> v = {{26, 2},
                                                {99, 5},
                                                {1, 3},
                                                {5, 1},
                                                {0, 2},
                                                {18, 1},
                                                {92, 6},
                                                {66, 4},
                                                {11, 1},
                                                {22, 2}};

    // auto cmp = [](const auto& lhs, const auto& rhs) {
    //     return lhs.second < rhs.second;
    // };

    struct Compare {
        bool operator()(std::pair<size_t, size_t>& lhs, std::pair<size_t, size_t>& rhs) {
            return lhs.second < rhs.second;
        }
    };

    std::priority_queue<std::pair<size_t, size_t>,
                        std::vector<std::pair<size_t, size_t>>,
                        Compare> docid_to_count;

    for (const auto& p : v) {
        docid_to_count.push(p);
    }

    size_t N = docid_to_count.size();
    for (size_t i = 0; i < N; ++i) {
        auto p = docid_to_count.top();
        std::cout << "{" << p.first << ", " << p.second << "}" << std::endl;
        docid_to_count.pop();
    }

    return 0;
}
