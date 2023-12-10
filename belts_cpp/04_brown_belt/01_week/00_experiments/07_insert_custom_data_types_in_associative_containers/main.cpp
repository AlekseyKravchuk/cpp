#include <set>
#include <unordered_set>

#include "generator.h"
#include "print.h"
#include "profile.h"
#include "hasher.h"


int main() {
    PlateGenerator pg;
    std::set<Plate> s_plates;
    std::unordered_set<Plate, PlateHasher> h_plates;
    const int N = 300'000;

    // Plate p = {'M', 463, 'C', 'A', 24};
    // h_plates.insert(p);

    {
        LOG_DURATION("set");
        for (int i = 0; i < N; ++i) {
            s_plates.insert(pg.GetRandomPlate());
        }

        for (int i = 0; i < N; ++i) {
            s_plates.find(pg.GetRandomPlate());
        }
    }

    {
        LOG_DURATION("unordered_set");
        for (int i = 0; i < N; ++i) {
            h_plates.insert(pg.GetRandomPlate());
        }

        for (int i = 0; i < N; ++i) {
            h_plates.find(pg.GetRandomPlate());
        }
    }

    // std::cout << std::left << std::setw(18) << "set: " << s_plates << std::endl;

    // for (int i = 0; i < N; ++i) {
    //     h_plates.insert(pg.GetRandomPlate());
    // }
    // std::cout << std::left << std::setw(18) << "unordered_set: " << h_plates << std::endl;

    return 0;
}
