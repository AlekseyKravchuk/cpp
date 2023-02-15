#include <cstddef>

template <typename T, size_t size>
class MyArray {
   private:
    T ar[size]{};  // обеспечиваем инициализацию элементов базового типа по умолчанию нулем посредством "{}"

   public:
    MyArray() = default;
    MyArray(const T *) { }

    ~MyArray() = default;
};

template <typename T, size_t size>
MyArray(T const (&arr)[size]) -> MyArray<T, size>;
