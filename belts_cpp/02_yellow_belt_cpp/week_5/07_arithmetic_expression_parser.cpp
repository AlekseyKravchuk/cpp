// https://www.youtube.com/watch?v=ISVIrpT3AN8&list=PL6-8bkaLoP42NzUkQcTS4D8kNQC12xCC-&index=9
#include <iostream>
#include <string>

using namespace std::literals;

class Node {
   public:
    int Evaluate() {
        return 0;
    }

    // TODO!
    Node Parse(const std::string& tokens, int& x) {
       public:
        Digit(int d) : _d(d) {}

       private:
        ind _d{};
    }
};

class Digit {
};

int main() {
    // std::string tokens;
    std::string tokens = "5 + 7 - x*x + x"s;
    // std::cout << "Type in expression: ";
    // std::cin >> tokens;

    // int x{};
    // auto expr = Parse(tokens, x);

    std::cout << "Enter x: "s;
    while (std::cin >> x) {
        std::cout << expr->Evaluate() << std::endl;
    }

    return 0;
}
