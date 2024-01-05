#include <bitset>
#include <iostream>
#include <iomanip>

using namespace std;;

int main() {
    cout << setw(2) << std::left << "x" << " = "  << bitset<8>(1<<6) << endl;
    cout << setw(2) << std::left << "~x" << " = " << bitset<8>(~(1<<6)) << endl;
    
    return 0;
}
