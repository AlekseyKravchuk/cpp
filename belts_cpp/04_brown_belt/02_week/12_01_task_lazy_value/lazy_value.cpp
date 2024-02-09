#include "test_runner.h"

#include <functional>
#include <optional>
#include <string>
#include <utility>

using namespace std;

template <typename T>
class LazyValue {
  public:
    explicit LazyValue(std::function<T()> init)
        : _init_callable(std::move(init)) {}

    bool HasValue() const {
        return _value_holder.has_value();
    }

    const T& Get() const {
        if (!HasValue()) {
            _value_holder = _init_callable();
        }
        return *_value_holder;
    }

  private:
    mutable std::optional<T> _value_holder; // поля, помеченные "mutable", можно изменять в константных методах
    std::function<T()> _init_callable;
};

void UseExample() {
    const string big_string = "Giant amounts of memory";

    LazyValue<string> lazy_string([&big_string]() {
        return big_string;
    });

    ASSERT(!lazy_string.HasValue());
    ASSERT_EQUAL(lazy_string.Get(), big_string);
    ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called]() {
            called = true;
            return 0;
        });
    }
    ASSERT(!called);
}

void TestInitializerCalled() {
    bool called = false;

    LazyValue<int> lazy_int([&called]() {
        called = true;
        return 55;
    });

    ASSERT(!called);

    int value = lazy_int.Get();
    ASSERT(called);
    ASSERT_EQUAL(value, 55);
}

void TestInitializerCalledJustOnce() {
    int count = 0;
    LazyValue<int> lazy_int([&count]() {
        ++count;
        return 0;
    });

    ASSERT_EQUAL(count, 0);

    const int& int_ref_1 = lazy_int.Get();
    ASSERT_EQUAL(count, 1);

    const int& int_ref_2 = lazy_int.Get();
    ASSERT_EQUAL(count, 1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, UseExample);
    RUN_TEST(tr, TestInitializerIsntCalled);
    RUN_TEST(tr, TestInitializerCalled);
    RUN_TEST(tr, TestInitializerCalledJustOnce);
    return 0;
}
