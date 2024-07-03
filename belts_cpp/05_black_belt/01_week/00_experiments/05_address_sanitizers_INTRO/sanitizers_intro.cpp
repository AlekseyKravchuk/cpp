#include <iostream>
#include <string>
#include <vector>

using namespace std;

// In order "clang++-17" can find headers "libstdc++-12-dev"  should be installed
// sudo apt install libstdc++-12-dev
// Path to clang++: /usr/bin/clang-cpp-17
// Command to compile:
// clang++-17 -std=c++17  sanitizers_intro.cpp -o sanitizers_intro -g -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address
// clang++-17 -g -std=c++17 -O1 sanitizers_intro.cpp -o sanitizers_intro -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address

// Перед запуском программы указываем переменную окружению с путем к symbolizer'у.
// Сперва этот путь нужно найти:
// dpkg -L llvm-17 | grep symbolize
// OUTPUT:
// /usr/lib/llvm-17/bin/llvm-symbolizer
// /usr/share/man/man1/llvm-symbolizer-17.1.gz
// /usr/bin/llvm-symbolizer-17

// Запускаем скомпилированную программу :
// ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-17/bin/llvm-symbolizer ./sanitizers_intro

const string& MakeString() {
  const vector<string> strings = {
    "C++",
    "Python"
  };
    
  return strings[0];
}

int main() {
  const auto& ref = MakeString();
  cout << ref << endl;
  return 0;
}
