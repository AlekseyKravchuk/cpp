#include <iostream>

// function printing n numbers in reverse order: n, n-1, n-2, ..., 2, 1 without using a loop
// it's tail recursive function because recursive call is the last thing that happens in the function
void reverse_order(int n) {
  if(n < 1) {
    return;
  }
  else {
    std::cout << n << " " << std::flush;
    reverse_order(n-1);
  }
}


int main() {
  int n{0};
  std::cout << "Type in n: ";
  std::cin >> n;

  reverse_order(n);
  std::cout << std::endl << std::flush;

  return 0;
}
