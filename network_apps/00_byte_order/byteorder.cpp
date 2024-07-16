#include <cstdlib>  // exit, EXIT_FAILURE
#include <iostream> // printf

using namespace std;

int main() {
  union {
    short _short;
    char _char_arr[sizeof(short)];
  } un;

  un._short = 0x0102;

  if (un._char_arr[0] == 0x01 && un._char_arr[1] == 0x02) {
    cout << "big-endian" << endl;
  } else if (un._char_arr[0] == 0x02 && un._char_arr[1] == 0x01) {
    cout << "little-endian" << endl;
  } else {
    cout << "unknown" << endl;
  }

  exit(EXIT_FAILURE);
}
