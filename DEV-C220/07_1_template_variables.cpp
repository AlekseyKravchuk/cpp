template <typename T>
class A {
    T _t;
    static size_t _count;

   public:
    // совмещаем дефолтный конструктор и конструктор с параметром
    // дефолтным конструктором называется конструктор, у которого нет параметров или все параметры заданы по умолчанию
    A(const T& t = T{}) : _t(t) {
        ++_count;
    }

    // также учет экземляров класса нужно реализовать в конструкторе копирования
    A(const T& other) {
        ++_count;
    }

    ~A() {
        --_count;
    }

    static size_t GetCount() {
        return _count;
    }
};

int main(int argc, char const* argv[]) {
    /* code */
    return 0;
}
