#include <iostream>


long long fibNum(int n) {

  // if(n == 0) return 0;
  // if(n == 1) return 1;

  // this is shortcut for the 2 above lines of code
  if(n <= 1) {
    return n;
  }

  return fibNum(n-1) + fibNum(n-2);
}


int main() {
  int n{0};
  std::cout << "Type n to compute Fibonacci Number Fn: ";
  std::cin >> n;

  std::cout << "F(" << n << ") = " << fibNum(n) << std::endl;

  return 0;
}
