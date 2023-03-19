#include <cassert>

template <typename T>
inline int sgn(T val) {
    return ((T{} < val) - (val < T{}));
}

void TestSGN() {
    assert(0 == sgn(0));
    assert(0 == sgn(0.0));
    assert(0 == sgn(0.0f));
    assert(1 == sgn(1.3f));
    assert(1 == sgn(110));
    assert(1 == sgn(0.0001f));
    assert(1 == sgn(0.5));
    assert(-1 == sgn(-0.0003));
    assert(-1 == sgn(-9.0f));
    assert(-1 == sgn(-234234));
    assert(-1 == sgn(-1.00003f));
}