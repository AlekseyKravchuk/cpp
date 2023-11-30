#include <iostream>

// function printing n numbers in direct order: n, n-1, n-2, ..., 2, 1 without using a loop
void direct_order(int n) {
  if(n < 1) {
    return;
  }
  else {
    direct_order(n-1);
    std::cout << n << " " << std::flush;
  }
}


int main() {
  int n{0};
  std::cout << "Type in n: ";
  std::cin >> n;

  direct_order(n);
  std::cout << std::endl << std::flush;

  return 0;
}
