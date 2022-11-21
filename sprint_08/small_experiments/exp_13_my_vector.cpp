#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

template <typename T>
class SimpleVector {
   public:
    explicit SimpleVector(size_t size) : _size(size) {
        _data = new T[size];
        _pastTheEnd = _data + size;
    }

    // конструктор, принимающий std::initializer_list<Type> для инициализации списка заранее предоставленными элементами
    SimpleVector(std::initializer_list<T> values) : _size(values.size()) {
        try {
            _data = new T[_size];
            _pastTheEnd = _data + _size;

            size_t i = 0;
            for (auto it = std::begin(values); it != std::end(values); ++it) {
                _data[i++] = *it;
            }

        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            throw;
        }
    }

    ~SimpleVector() {
        delete[] _data;
    }

    // оператор доступа к элементу массива по индексу
    T& operator[](size_t index) {
        return *(_data + index);
    }

    // когда вектор передан по КОНСТАНТНОЙ ссылке вызываются эти 2 пары методов
    // эти методы вызвращают указатели, запрещающие изменять объекты, на которые они указывают
    const T* begin() const { return _data; }
    const T* end() const { return _pastTheEnd; }

    // НЕКОНСТАНТНЫЕ методы begin() и end() будут вызываться для неконстантных объектов класса SimpleVector
    // т.е. в том случае, если мы обращаемся к объекту по НЕконстантной ссылке или просто обращаемся через объект класса SimpleVector
    T* begin() { return _data; }
    T* end() { return _pastTheEnd; }

   private:
    // указатель на первый элемент нашего вектора
    size_t _size;
    T* _data;
    T* _pastTheEnd;
};

template <typename Type>
void Print(const SimpleVector<Type>& v) {
    // вектор "v" мы передаем по КОНСТАНТНОЙ ссылке, а раз ссылка КОНСТАНТНАЯ, то мы можем вызывать ТОЛЬКО КОНСТАНТНЫЕ МЕТОДЫ
    // у объеекта, на который ссылается ссылка "v"
    // НО! методы SimpleVector::begin() и SimpleVector::end() не являются константными, иными словами
    // мы никак не указали компилятору, что SimpleVector::begin() и SimpleVector::end() никак не изменяют внутреннего устройства ветора
    // т.е. компилятор считает, что вызов SimpleVector::begin() или SimpleVector::end() может изменить внутреннее состояние вектора
    // и, поэтому, запрещает нам это сделать
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " "s;
    }

    std::cout << std::endl;
}

int main() {
    SimpleVector<int> v = {1, 2, 3, 4, 5};

    PrintRange(v.begin(), v.end());

    v[0] = 22;
    v[1] = 2;
    v[2] = 10;
    v[3] = 3;
    v[4] = 1;

    for (auto elm : v) {
        std::cout << elm << " "s;
    }
    std::cout << std::endl;

    std::sort(v.begin(), v.end());

    // PrintRange(v.begin(), v.end());
    Print(v);
    Print(v);

    return 0;
}
