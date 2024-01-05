#include <cstddef>
#include <iostream>

template <typename Derived>
class CRTPInterface {
   public:
    void Count(uint64_t n) {
        static_cast<Derived*>(this)->Count(n);
    }

    uint64_t GetValue() {
        return static_cast<Derived*>(this)->GetValue();
    }
};

class CRTPImplemented : public CRTPInterface<CRTPImplemented> {
   public:
    CRTPImplemented() : _counter(0) { }

    void Count(uint64_t n) {
        _counter += n;
    }

    uint64_t GetValue() {
        return _counter;
    }

   private:
    uint64_t _counter;
};

template <typename T>
void RunCRTP(CRTPInterface<T>* obj) {
    const unsigned N = 40000;
    for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < i; ++j) {
            obj->Count(j);
        }
    }

    std::cout << obj->GetValue() << std::endl;
}