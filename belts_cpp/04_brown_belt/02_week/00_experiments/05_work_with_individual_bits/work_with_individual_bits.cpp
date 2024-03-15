#include <bitset>
#include <iostream>
#include <iomanip>
#include <bit>

using namespace std;

int main() {
    cout << setw(2) << std::left << "x" << " = " << bitset<8>(1u << 6) << endl;
    cout << setw(2) << std::left << "~x" << " = " << bitset<8>(~(1u << 6)) << endl;

     bitset<8> bs(4);
    cout << "number of ones in " << bs << " = " << bs.count() << endl;

    // TODO: найти способ быстро находить индекс первой 1 в битовом представлении числа
//    cout << "index of first 1 in " << bs << "(bitset<8>)" << " = " << __builtin_ffs(uint8_t (bs)) << endl;
    // cout << "number of ones in " << bitset<8>(37) << " = " << bitset<8>(37).count() << endl;


    return 0;
}
