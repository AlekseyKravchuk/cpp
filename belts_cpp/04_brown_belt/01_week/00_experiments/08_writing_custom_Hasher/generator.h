#include "plates.h"
#include <array>
#include <random>

class PlateGenerator {
  public:
    Plate GetRandomPlate() {
        Plate p;
        p.C1 = Letters[LetterDist(RandEng)];
        p.Number = NumberDist(RandEng);
        p.C2 = Letters[LetterDist(RandEng)];
        p.C3 = Letters[LetterDist(RandEng)];
        p.Region = RegionDist(RandEng);

        return p;
    }

  private:
    const static int N = 12;
    const std::array<char, N> Letters = {
        'A', 'B', 'C', 'E', 'H', 'K', 'M', 'O', 'P', 'T', 'X', 'Y'};  // 12 букв, которые могут использоваться в автомобильных номерах

    std::default_random_engine RandEng;
    std::uniform_int_distribution<int> LetterDist{0, N - 1};
    std::uniform_int_distribution<int> NumberDist{1, 999};
    std::uniform_int_distribution<int> RegionDist{1, 99};
};