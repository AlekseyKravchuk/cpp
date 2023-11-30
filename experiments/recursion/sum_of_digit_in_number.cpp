// implementation of recursive function that finds sum of digits in a number
// Input: n = 253
// Output: 10 (2+5+3)
#include <iostream>

int getSumOfDigitsRecursively(int n) {
  if(n < 10) {
    return n;
  }
  return (n % 10) + getSumOfDigitsRecursively(n/10);
}

int main() {
  int num;
  std::cout << "Type in the number to get the sum of its digits: ";
  std::cin >> num;
  std::cout << "Sum of digits = " << getSumOfDigitsRecursively(num) << std::endl;
  return 0;
}
