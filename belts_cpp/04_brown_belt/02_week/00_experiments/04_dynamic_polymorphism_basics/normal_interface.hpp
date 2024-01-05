#include <cstddef>
#include <iostream>

class NormalInterface {
   public:
    virtual void Count(uint64_t n) = 0;
    virtual uint64_t GetValue() const = 0;
};

class NormalImplemented : public NormalInterface {
   public:
    NormalImplemented() : _counter(0) {}

    virtual void Count(uint64_t n) override {
        _counter += n;
    }

    virtual uint64_t GetValue() const override {
        return _counter;
    }

   private:
    uint64_t _counter;
};

void RunNormal(NormalInterface* obj) {
    const unsigned N = 40'000;
    // const unsigned N = 10;
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < i; ++j) {
            obj->Count(j);
        }
    }

    std::cout << obj->GetValue() << std::endl;
}

