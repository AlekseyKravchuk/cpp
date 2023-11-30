// This program implements recursive calculation of factorial

#include <iostream>

long long fact(unsigned int n) {
  if(n == 0) { // base case
    return 1;
  }
  else {
    return n*fact(n-1);
  }
}


int main() {
  int n{0};
  std::cout << "Type in number, n to calculate n!: ";
  std::cin >> n;
  std::cout << n << "! = " << fact(n) << std::endl;
  return 0;
}
