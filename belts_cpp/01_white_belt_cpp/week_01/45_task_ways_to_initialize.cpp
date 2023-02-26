#include <algorithm>
#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

/*

Задание по программированию «Способы инициализации»

Определите тип Incognizable, для которого следующий код будет корректен:

int main() {
  Incognizable a;
  Incognizable b = {};
  Incognizable c = {0};
  Incognizable d = {0, 1};
  return 0;
}
*/

class Incognizable {
   private:
    int _a{};
    int _b{};

   public:
    Incognizable() = default;
    Incognizable(int a, int b = 0) : _a(a), _b(b) {}
};

int main() {
    Incognizable a;
    Incognizable b = {};
    Incognizable c = {0};
    Incognizable d = {0, 1};
    
    return 0;
}