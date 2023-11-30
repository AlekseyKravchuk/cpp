#include <iostream>


// output numbers in reverse order without using loop
void reverse_order(int n) {
  if(n < 1) {
    return;
  }
  else {
    std::cout << n << " " << std::flush;
    reverse_order(n-1);
  }
}


void direct_order(int n) {
  if(n < 1) {
    return;
  }

}


int main() {
  reverse_order(5);
  std::cout << std::endl;
  return 0;
}
