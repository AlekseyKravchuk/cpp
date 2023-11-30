#include <iostream>

void fun(int n) {
  if(n < 1) {
    std::cout << "   " << std::flush;
    return;
  }  else {
    std::cout << n << " " << std::flush;
    fun(n-1);
    std::cout << n << " " << std::flush;
  }
}


int main() {
  fun(5);
  std::cout << std::endl;
  return 0;
}
