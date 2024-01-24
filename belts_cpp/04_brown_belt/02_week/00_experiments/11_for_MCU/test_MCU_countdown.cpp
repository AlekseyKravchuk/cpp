#include <cassert>
#include <iostream>

#include "test_runner.h"

using namespace std;

int digits[4] = {0, 0, 0, 0};
int start = 0;

void ResetDigits(int* digits);

void GetDigitsFromNumber(int number, int* digits, const int digits_len) {
    assert(digits_len == 4);
	
	if (!number) {
		return;
	}

    int i = 3;
    while (number) {
        digits[i--] = number % 10;
        number /= 10;
    }
}

int GetIndexOfFirstNonZeroDigit(int* digits) {
	int i = 0;
    if (digits[i] == 0) {
        ++i;
        while (!digits[i] && i != 3) {
            ++i;
        }
    }

    return i;
}

void ResetDigits(int* digits) {
	for (int i = 0; i < 4; ++i) {
		digits[i] = 0;
	}
}

void Test_GetIndexOfFirstNonZeroDigit_4digits() {
    {
		ResetDigits(digits);
        int number = 7965;
        int start_index_expected = 0;
        int first_digit_expected = 7;

        GetDigitsFromNumber(number, digits, 4);
        int start_index = GetIndexOfFirstNonZeroDigit(digits);

        ASSERT_EQUAL(start_index_expected, start_index);
        ASSERT_EQUAL(first_digit_expected, digits[start_index]);
    }
}

void Test_GetIndexOfFirstNonZeroDigit_3digits() {
	ResetDigits(digits);
    int number = 965;
    int start_index_expected = 1;
    int first_digit_expected = 9;

    GetDigitsFromNumber(number, digits, 4);
    int start_index = GetIndexOfFirstNonZeroDigit(digits);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_2digits() {
	ResetDigits(digits);
    int number = 65;
    int start_index_expected = 2;
    int first_digit_expected = 6;

    GetDigitsFromNumber(number, digits, 4);
    int start_index = GetIndexOfFirstNonZeroDigit(digits);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_1digit() {
	ResetDigits(digits);
    int number = 5;
    int start_index_expected = 3;
    int first_digit_expected = 5;

    GetDigitsFromNumber(number, digits, 4);
    int start_index = GetIndexOfFirstNonZeroDigit(digits);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_All_Zeros() {
	ResetDigits(digits);
    int number = 0;
    int start_index_expected = 3;
    int first_digit_expected = 0;

    GetDigitsFromNumber(number, digits, 4);
    int start_index = GetIndexOfFirstNonZeroDigit(digits);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits[start_index]);
}

void Test_DigitsFromNumber_4digits() {
    ResetDigits(digits);

    int number = 2469;
    GetDigitsFromNumber(number, digits, 4);

    int digits_expected[4] = {2, 4, 6, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_3digits() {
    ResetDigits(digits);

    int number = 469;
    GetDigitsFromNumber(number, digits, 4);

    int digits_expected[4] = {0, 4, 6, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_2digits() {
    ResetDigits(digits);

    int number = 69;
    GetDigitsFromNumber(number, digits, 4);

    int digits_expected[4] = {0, 0, 6, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_1digits() {
    ResetDigits(digits);

    int number = 9;
    GetDigitsFromNumber(number, digits, 4);

    int digits_expected[4] = {0, 0, 0, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_all_zeros() {
    ResetDigits(digits);

    int number = 0;
    GetDigitsFromNumber(number, digits, 4);

    int digits_expected[4] = {0, 0, 0, 0};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits[i], digits_expected[i]);
    }
}



int main() {
    TestRunner tr;
    RUN_TEST(tr, Test_GetIndexOfFirstNonZeroDigit_4digits);
    RUN_TEST(tr, Test_GetIndexOfFirstNonZeroDigit_3digits);
	RUN_TEST(tr, Test_GetIndexOfFirstNonZeroDigit_2digits);
	RUN_TEST(tr, Test_GetIndexOfFirstNonZeroDigit_1digit);
	RUN_TEST(tr, Test_GetIndexOfFirstNonZeroDigit_All_Zeros);

    // =====================================================
    RUN_TEST(tr, Test_DigitsFromNumber_4digits);
    RUN_TEST(tr, Test_DigitsFromNumber_3digits);
    RUN_TEST(tr, Test_DigitsFromNumber_2digits);
    RUN_TEST(tr, Test_DigitsFromNumber_1digits);
    RUN_TEST(tr, Test_DigitsFromNumber_all_zeros);
    // =====================================================
	
    // int number = 7965;
    // GetDigitsFromNumber(number, digits, 4);
    // start = GetIndexOfFirstNonZeroDigit(digits);
    // std::cout << "start_index = " << start << std::endl;
    // std::cout << "digits[start] = " << digits[start] << std::endl;
    return 0;
}
