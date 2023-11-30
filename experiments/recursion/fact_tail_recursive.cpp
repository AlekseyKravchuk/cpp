// This program implements tail recursive calculation of factorial
#include <iostream>


long long fact_tail_recursive(unsigned int n, unsigned int val=1) {
  // static int counter{0};
  // counter++;
  // std::cout << "counter = " << counter << std::endl;

  if(n == 1) { // base case
    return val;
  }
  else {
    return fact_tail_recursive(n-1, n * val);
  }
}


int main() {
  int n{0};
  std::cout << "Type in number, n to calculate n!: ";
  std::cin >> n;
  long long res = fact_tail_recursive(n);
  std::cout << n << "! = " << res << std::endl;
  return 0;
}
