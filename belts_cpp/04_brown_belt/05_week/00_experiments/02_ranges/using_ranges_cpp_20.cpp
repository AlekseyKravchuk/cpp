#include <iostream>
#include <ranges>

//namespace rng = std::ranges;
//namespace view = rng::views;

int main() {
    const int days = 3;   // количество дней с играми
    const int games = 2;  // количество игр с питомцем в день

    for (int i: std::ranges::views::iota(0, days)) {
        std::cout << "День " << i << std::endl;
        for (int j: std::ranges::views::iota(0, games)) {
            std::cout << "  Игра " << j << std::endl;
        }
    }
}