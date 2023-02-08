#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    int n = 7, guess{-15};
    do {
        std::cout << "Try to guess the number in range [0, 9]. Type in your guess: ";
        std::cin >> guess;
    } while (n != guess);

    std::cout << "You are right"s << std::endl;

    return 0;
}
