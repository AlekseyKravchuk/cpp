#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
  Compile program:
clang++-17 -std=c++17 find_memory_leak_fsanitize.cpp -o find_memory_leak_fsanitize -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address

  Запускаем скомпилированную программу :
ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-17/bin/llvm-symbolizer ./find_memory_leak_fsanitize
*/

int main() {
  int* p = new int;

  return 0;
}
