#include <fstream>
#include <string>

using namespace std::literals;

int main() {
    std::ofstream output("nums.txt"s);
    bool isFirst = true;
    for (int i = 1; i <= 200'000; ++i) {
        if (isFirst) {
            output << i;
            isFirst = false;
        } else {
            output << ' ' << i;
        }
    }
    return 0;
}
