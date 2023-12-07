#include <cmath> // std::abs
#include <iostream>
#include <stack>
#include <string> // std::stoi
#include <vector>

#include "print.h"

// template <typename IntegerType>
// std::vector<IntegerType> ExtractDigitsFromNumber(IntegerType num) {
//     std::vector<IntegerType> result;

//     std::string num_str = std::to_string(num);
//     for (size_t i = 0; i < num_str.size(); ++i) {
//         result.push_back(num_str[i] - '0');
//     }

//     return result;
// }

template <typename IntegerType>
std::vector<IntegerType> ExtractDigitsFromNumber(IntegerType num) {
    std::vector<IntegerType> result;
    if (!num) {
        result.push_back(0);
        return result;
    }

    auto i = std::abs(num);
    std::stack<IntegerType> stack;

    while (i != 0) {
        auto digit = i % 10;
        i /= 10;
        stack.push(digit);
    }

    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}

void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void GetDigitsFrom4DigitsNumber(int number, int* arr, const int arr_len = 4) {
    for (int i = arr_len - 1; i >= 0; --i) {
        int digit = number % 10;
        arr[i] = digit;
        number /= 10;
    }
}

template <typename ArrayType>
void PrintPureArray(ArrayType* arr, size_t arr_len) {
    std::cout << "[";
    for (size_t i = 0; i < arr_len; ++i) {
        if (i) {
            std::cout << ", " << arr[i];
        } else {
            std::cout << arr[i];
        }
    }
    std::cout << "]";
}

int main() {
    // int num = 341188;
    // std::cout << "number = " << num << std::endl;
    // std::cout << "digits of given number: " << ExtractDigitsFromNumber(num) << std::endl;

    int four_digit_num = 2345;
    int arr[4];

    GetDigitsFrom4DigitsNumber(four_digit_num, arr);
    
    std::cout << "four_digit_num = " << four_digit_num << std::endl;
    std::cout << "digits of given number: ";
    PrintPureArray(arr, 4);
    std::cout << std::endl;

    return 0;
}
