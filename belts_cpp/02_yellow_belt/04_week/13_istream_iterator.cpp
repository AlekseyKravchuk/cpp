#include <fstream>
#include <iostream>
#include <iterator>  // std::istream_iterator
#include <ostream>
#include <sstream>  // std::ostringstream
#include <stdexcept>
#include <string>
#include <vector>

using namespace std::literals;

// положим, у нас имеется некоторая шаблонная функция "sum", которая суммирует элементы переданного ей диапазона
template <typename InputIt>
auto sum(InputIt begin, InputIt end) {
    auto sum{*begin};
    for (auto it = ++begin; it != end; ++it) {
        sum += *it;
    }
    return sum;
}

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = " "s) {
    bool isFirst = true;
    std::ostringstream oss;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << delimeter;
        }
        isFirst = false;
        oss << elm;
    }
    return oss.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "["s << Join(v, ", "s) << "]"s;
}

int main() {
    // Алгоритмы могут применяться как к объектам из контейнера, так и к объектам из потока

    // // мы можем использовать "std::istream_iterator<int>" для чтения целых чисел из консоли
    // std::cout << "Enter numbers separated by whitespace." << std::endl;
    // std::cout << "Press Ctrl+Z (for Windows) or Ctrl+D (for Linux) followed by Enter to stop." << std::endl;
    // std::istream_iterator<int> numbersIter = std::cin;
    // std::istream_iterator<int> endIter;
    // std::cout << sum(numbersIter, endIter) << std::endl;

    // Также мы можем использовать "std::istream_iterator<int>" для чтения целых чисел из файла
    std::string path = "13_input.txt";
    std::ifstream inFile(path);
    if (!inFile) {
        throw std::runtime_error(""s);
    }
    std::cout << "File '" << path << " is used to read numbers from" << std::endl;

    // параметр коструктора "istream_iterator<T>" является НЕКОНСТАНТНОЙ ссылкой, поэтому не получится инстанцировать "istream_iterator<T>"
    // с помощью временного объекта (файлового потока):
    // std::istream_iterator<int> numbersIter(std::ifstream(path));  //ERROR
    std::istream_iterator<int> numbersIter(inFile);

    // конструктор без аргументов для "istream_iterator<T>" создает "end-of-stream iterator", являющийся аналогом "past-the-end iterator"
    std::istream_iterator<int> endIter;

    // Объекты считываются из файлового потока ввода посредством оператора "operator>>",
    // поэтому "operator>>" должен быть определен для данного типа
    std::vector<int> v(numbersIter, endIter);
    std::cout << "vector v: "s << v << std::endl;

    std::cout << "sum of vector elements: "s << sum(v.begin(), v.end()) << std::endl;

    return 0;
}
