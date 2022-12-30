#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    std::map<std::string, int> names =
        {
            {"Viktor"s, 32},
            {"Helen"s, 28},
            {"Aleks"s, 35},
            {"Olga"s, 31},
            {"Mike"s, 30}
        };

    return 0;
}
