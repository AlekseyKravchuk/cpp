// C++ Program to convert string into integer using stoi
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// Driver Code
int main(int argc, char* argv[]) {
    std::string arg = argv[1];
    int x = 0;

    try {
        std::size_t pos;
        x = std::stoi(arg, &pos);

        if (pos < arg.size()) {
            std::cerr << "Trailing characters after number: " << arg << '\n';
        }
    } catch (std::invalid_argument const& ex) {
        std::cerr << "Invalid number: " << arg << '\n';
    } catch (std::out_of_range const& ex) {
        std::cerr << "Number out of range: " << arg << '\n';
    }

    cout << "x = " << x << endl;

    return 0;
}
 