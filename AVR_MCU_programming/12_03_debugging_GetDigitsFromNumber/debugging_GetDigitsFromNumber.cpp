#include <cassert>
#include <iostream>

#include "test_runner.h"

using namespace std;

int digits_of_number[4] = {0, 0, 0, 0};
const size_t d_length = sizeof(digits_of_number) / sizeof(int); // length == 4

int start = 0;
int seconds_counter = 0;

void ResetDigits() {
    for (size_t i = 0; i < d_length; ++i) {
        digits_of_number[i] = 0;
    }
}

// returns the position of the first non-zero digit (or pos = d_length-1 if number == 0) in a "d_length"-digit number
int GetDigitsFromNumber(int number) {
	if (number == 0) {
		digits_of_number[d_length - 1] = 0;
		return d_length - 1;
	}

	int i = d_length - 1; // initial value of i == 3
	int pos = i;

	while (number) {
		digits_of_number[i--] = number % 10;
		number /= 10;

		if (number) {
			--pos;
		}
	}

	return pos;
}

void Test_GetIndexOfFirstNonZeroDigit_4digits() {
    {
        ResetDigits();
        int number = 7965;
        int start_index_expected = 0;
        int first_digit_expected = 7;

        int start_index = GetDigitsFromNumber(number);

        ASSERT_EQUAL(start_index_expected, start_index);
        ASSERT_EQUAL(first_digit_expected, digits_of_number[start_index]);
    }
}

void Test_GetIndexOfFirstNonZeroDigit_3digits() {
    ResetDigits();
    int number = 965;
    int start_index_expected = 1;
    int first_digit_expected = 9;

    int start_index = GetDigitsFromNumber(number);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits_of_number[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_2digits() {
    ResetDigits();
    int number = 65;
    int start_index_expected = 2;
    int first_digit_expected = 6;

    int start_index = GetDigitsFromNumber(number);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits_of_number[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_2digits_2() {
    ResetDigits();
    int number = 28;
    int start_index_expected = 2;
    int first_digit_expected = 2;

    int start_index = GetDigitsFromNumber(number);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits_of_number[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_1digit() {
    ResetDigits();
    int number = 5;
    int start_index_expected = 3;
    int first_digit_expected = 5;

    int start_index = GetDigitsFromNumber(number);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits_of_number[start_index]);
}

void Test_GetIndexOfFirstNonZeroDigit_All_Zeros() {
    ResetDigits();
    int number = 0;
    int start_index_expected = 3;
    int first_digit_expected = 0;

    int start_index = GetDigitsFromNumber(number);

    ASSERT_EQUAL(start_index_expected, start_index);
    ASSERT_EQUAL(first_digit_expected, digits_of_number[start_index]);
}

void Test_DigitsFromNumber_4digits() {
    ResetDigits();

    int number = 2469;
    GetDigitsFromNumber(number);

    int digits_expected[4] = {2, 4, 6, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits_of_number[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_3digits() {
    ResetDigits();

    int number = 469;
    GetDigitsFromNumber(number);

    int digits_expected[4] = {0, 4, 6, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits_of_number[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_2digits() {
    ResetDigits();

    int number = 69;
    GetDigitsFromNumber(number);

    int digits_expected[4] = {0, 0, 6, 9};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits_of_number[i], digits_expected[i]);
    }
}

void Test_DigitsFromNumber_1digits() {
    {
        ResetDigits();

        int number = 9;
        GetDigitsFromNumber(number);

        int digits_expected[4] = {0, 0, 0, 9};
        for (size_t i = 0; i < 4; ++i) {
            ASSERT_EQUAL(digits_of_number[i], digits_expected[i]);
        }
    }

    {
        ResetDigits();

        int number = 1;
        GetDigitsFromNumber(number);

        int digits_expected[4] = {0, 0, 0, 1};
        for (size_t i = 0; i < 4; ++i) {
            ASSERT_EQUAL(digits_of_number[i], digits_expected[i]);
        }
    }
}

void Test_DigitsFromNumber_all_zeros() {
    ResetDigits();

    int number = 0;
    GetDigitsFromNumber(number);

    int digits_expected[4] = {0, 0, 0, 0};
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(digits_of_number[i], digits_expected[i]);
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
    RUN_TEST(tr, Test_GetIndexOfFirstNonZeroDigit_2digits_2);
    // =====================================================

    // int number = 7965;
    // GetDigitsFromNumber(number, digits, 4);
    // start = GetIndexOfFirstNonZeroDigit(digits);
    // std::cout << "start_index = " << start << std::endl;
    // std::cout << "digits[start] = " << digits[start] << std::endl;
    return 0;
}
