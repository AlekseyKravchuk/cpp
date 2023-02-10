#include <algorithm>
#include <iostream>
#include <string>

/*
На вход дано целое положительное число N. Выведите его в двоичной системе счисления без ведущих нулей.
 */

// std::string DecimalToBinary(int n) {
//     std::string str{};
//     if (!n) {
//         return "0";
//     }

//     while (n != 0) {
//         str = std::to_string(n % 2) + str;
//         n /= 2;
//     }
//     return str;
// }

std::string DecimalToBinary(int n) {
    std::string str{};

    while (n != 0) {
        str += std::to_string(n % 2);
        n /= 2;
    }
    std::reverse(str.begin(), str.end());

    return str;
}

int main() {
    int n{};
    std::cin >> n;
    // std::cout << "decimal number " << n << " in binary: " << DecimalToBinary(n) << std::endl;
    std::cout << DecimalToBinary(n);

    return 0;
}
