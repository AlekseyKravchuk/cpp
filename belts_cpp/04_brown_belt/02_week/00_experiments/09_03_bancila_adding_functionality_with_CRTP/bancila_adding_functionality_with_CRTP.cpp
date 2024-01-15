/*
Предположим, что некоторые из наших игровых единиц ("game units") имеют следующие функции-члены:
- step_forth: передвинуть игровую единицу на 1 позицию вперед;
- step_back: передвинуть игровую единицу на 1 позицию назад;
*/
#include <atomic>  // std::atomic
#include <cstddef> // size_t
#include <iostream>
#include <stdexcept> // std::logic_error

// в самой минимально обвязке наши классы будут выглядеть следующим образом:
// struct knight {
//     void step_forth();
//     void step_back();
// };

// struct mage {
//     void step_forth();
//     void step_back();
// };

// Однако может ВДРУГ возникнуть требование, чтобы всё могло двигаться вперед или назад на ПРОИЗВОЛЬНОЕ ЧИСЛО ШАГОВ
// CRTP может нам помочь выкрутиться.

template <typename T>
struct movable_unit {
    void advance(size_t steps) {
        while (steps--) {
            static_cast<T*>(this)->step_forth();
        }
    }

    void retreat(size_t steps) {
        while (steps--) {
            static_cast<T*>(this)->step_back();
        }
    }
};

struct knight : public movable_unit<knight> {
    void step_forth() {
        std::cout << "knight moves forward\n";
    }

    void step_back() {
        std::cout << "knight moves back\n";
    }
};

struct mage : public movable_unit<mage> {
    void step_forth() {
        std::cout << "mage moves forward\n";
    }

    void step_back() {
        std::cout << "mage moves back\n";
    }
};

int main() {
    knight k;
    k.advance(3);
    std::cout << "=============================" << std::endl;
    k.retreat(2);
    std::cout << "=============================" << std::endl;
    mage m;
    m.advance(4);
    m.retreat(2);

    return 0;
}
