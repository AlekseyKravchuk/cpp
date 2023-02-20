/*
================= Заготовка для классов с подсчетом ссылок =================
*/

#include <cstddef>  // size_t
#include <cstring>  // std::strlen, std::strcpy

class Counter {  // вспомогательный
   private:
    size_t _owners_num{1};
    char* _pStr;

   private:
    Counter(const char* str);
    void AddOwner();
    void RemoveOwner();
    ~Counter();
    friend class MyString;
};

Counter::Counter(const char* str) {
    _pStr = new char[std::strlen(str) + 1];
    std::strcpy(_pStr, str);
}

Counter::~Counter() {
    delete[] _pStr;
}

void Counter::AddOwner() {
    ++_owners_num;
}

void Counter::RemoveOwner() {
    --_owners_num;
    if (_owners_num == 0) {
        delete this;
    }
}

class MyString {  // основной класс
   private:
    // вместо указателя на РЕСУРС держим адрес (указатель) объекта-обертки "Counter"
    Counter* _pCounter{nullptr};

   public:
    MyString(const char* str);
    MyString(const MyString& other);
    MyString& operator=(const MyString& other);
    ~MyString();
};

// конструктор принимает указатель на строчку
MyString::MyString(const char* str) {
    if (str) {
        // первый владелец выделяет ресурс
        _pCounter = new Counter(str);
    }
}

MyString::~MyString() {
    if (_pCounter) {
        _pCounter->RemoveOwner();
    }
}

// классический конструктор копирования
MyString::MyString(const MyString& other) {
    _pCounter = other._pCounter;
    if (_pCounter) {
        // учитываем ещё одного владельца, который подключился к пользованию ресурсом
        _pCounter->AddOwner();
    }
}

// оператор присваивания
MyString& MyString::operator=(const MyString& other) {
    if (_pCounter != other._pCounter) {
        if (_pCounter) {
            // отказываемся от своего ресурса
            _pCounter->RemoveOwner();
        }

        if (other._pCounter) {
            _pCounter = other._pCounter;
            _pCounter->AddOwner();
        }
    }
}

// TODO:
// move-операции
// GetString()
// SetNewString()
// operator<<