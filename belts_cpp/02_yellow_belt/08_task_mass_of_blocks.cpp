#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iostream>
#include <limits>
#include <string>

using namespace std::literals;

int main() {
    unsigned N, R;
    std::cin >> N >> R;
    uint64_t total_mass{};

    for (size_t i = 0; i < N; ++i) {
        uint64_t W, H, D, V;
        std::cin >> W >> H >> D;
        V = W * H * D;
        total_mass += V * R;
    }

    std::cout << total_mass << std::endl;

    return 0;
}
