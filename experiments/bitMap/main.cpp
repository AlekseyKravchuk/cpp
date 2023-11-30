//============================================================================
// Name        : main.cpp
// Author      : Aleksey Kravchuk
//============================================================================

#include <iostream>



#include "Bitmap.h"

using namespace std;
using namespace bmp;

int main() {
  int width{32};
  int height{32};

  Bitmap bitmap(width, height);

  for(auto y = 0; y < height; y++ ) {
    for(auto x = 0; x < width; x++) {
      if(y < height/3) {
        bitmap.setPixel(x, y, 255, 0, 0); // red
      }
      else if(y < 2*height/3) {
        bitmap.setPixel(x, y, 0, 0, 255); // blue
      }
      else {
        bitmap.setPixel(x, y, 255, 255, 255); // white
      }
    }

  }
  bitmap.write("test.bmp");

  // cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
  return 0;
}
