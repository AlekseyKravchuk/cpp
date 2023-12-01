#pragma once

#include <mutex>
#include <utility>

template <typename T>
class Synchronized {
  public:
    explicit Synchronized(T initial = T())
        : _value(std::move(initial)) {}

    struct Access {
        T& ref_to_value;
        std::lock_guard<std::mutex> guard;
    };

    Access GetAccess() {
        return {_value, std::lock_guard(_m)};
    }

  private:
    T _value;
    std::mutex _m;
};
