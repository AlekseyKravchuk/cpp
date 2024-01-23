#include <iostream>
#include <cassert>

using namespace std;


int digits[4] = {0, 0, 0, 0};
int start = 0;

void GetDigitsFromNumber(int number, int* digits, const int digits_len) {
	assert(digits_len == 4);
	int i = 3;
	while (number) {
		digits[i--] = number % 10;
		number /= 10;
	}
}

int GetIndexOfFirstNonZeroDigit(int* digits) {
    int i = 1;
	if(digits[0] == 0) {
		while(!digits[i] && i != 3) {
            ++i;
        }
	}
	
	return i;
}

int main() {
    int number = 290;
    GetDigitsFromNumber(number, digits, 4);
    start = GetIndexOfFirstNonZeroDigit(digits);
    std::cout << "start_index = " << start << std::endl;
    std::cout << "digits[start] = " << digits[start] << std::endl;
    return 0;
}
