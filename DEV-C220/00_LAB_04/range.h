#pragma once

#include <numeric>
#include <string>
#include <sstream>

template <typename T>
class RangeMinMax {
   private:
    T _min{};
    T _max{};

   public:
    constexpr RangeMinMax(T min, T max) : _min(min), _max(max) {}

    constexpr T GetMin() const {
        return _min;
    }

    constexpr T GetMax() const {
        return _max; 
    }

    constexpr bool IsInRange(const T val) const {
        return (val >= _min) && (val <= _max);
    }

    constexpr T CheckAndGet(const T val) const {
        if (!IsInRange(val)) {
            if (val < _min) {
                return _min;
            } else if (val > _max) {
                return _max;
            }
        }
        return val;
    }
};