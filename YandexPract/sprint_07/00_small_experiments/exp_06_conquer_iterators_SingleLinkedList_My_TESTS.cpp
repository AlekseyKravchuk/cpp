#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>  // std::initializer_list для инициализации путём передачи набора элементов напрямую в конструктор
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

template <typename Type>
class SingleLinkedList {
   private:
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), nextNode(next) {}
        Type value;
        Node* nextNode = nullptr;
    };

    // Шаблон класса «Базовый Итератор». Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
       public:
        // Класс списка объявляется дружественным, чтобы из методов списка был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;

        // Тип элементов, по которым перемещается итератор
        using value_type = Type;

        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;

        // Тип указателя на итерируемое значение
        using pointer = ValueType*;

        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) : _node(node) {}
        // explicit BasicIterator(Node* node) : _node(node) {
        //     std::cout << "Converting constructor for class BasicIterator is called." << std::endl;
        // }

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept : _node(other._node) {}
        // BasicIterator(const BasicIterator<Type>& other) noexcept : _node(other._node) {
        //     std::cout << "Copy constructor for class BasicIterator is called." << std::endl;
        // }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return _node == rhs._node;
        }

        // Оператор проверки итераторов на неравенство
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return _node != rhs._node;
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return _node == rhs._node;
        }

        // Оператор проверки итераторов на неравенство
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return _node != rhs._node;
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            if (_node != nullptr) {
                _node = _node->nextNode;
            }

            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            auto saved = *this;

            if (_node != nullptr) {
                _node = _node->nextNode;
            }

            return saved;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return _node->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            return &(_node->value);
        }

       private:
        Node* _node = nullptr;
    };

   public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;

    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(_head.nextNode);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Константный итератор эквивалентен указателю на константное значение
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(_head.nextNode);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(_head.nextNode);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    SingleLinkedList() = default;

    // конструктор, принимающий std::initializer_list<Type> для инициализации списка заранее предоставленными элементами
    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto rev_it = std::rbegin(values); rev_it != std::rend(values); ++rev_it) {
            PushFront(*rev_it);
        }

        _size = values.size();
    }

    // конструктор копирования
    SingleLinkedList(const SingleLinkedList& other) {
        // Сначала надо удостовериться, что текущий список пуст
        assert(_size == 0 && _head.nextNode == nullptr);

        // создаем пустой временный список
        SingleLinkedList tmp;

        // копируем элементы списка other во временный список
        for (auto it = other.begin(); it != other.end(); ++it) {
            std::cout << "OK" << std::endl;
        }
        //tmp._head.nextNode = ...

        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(tmp);
        // Теперь tmp пуст, а текущий список содержит копию элементов other

        assert(tmp._size == 0);
    }

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] std::size_t GetSize() const noexcept {
        return _size;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return _size == 0;
    }

    void PushFront(const Type& val) {
        Node* insertedNodePtr = new Node(val, _head.nextNode);
        _head.nextNode = insertedNodePtr;

        ++_size;
    }

    void Clear() noexcept {
        while (_head.nextNode) {
            Node* firstNodePtr = _head.nextNode;
            auto secondNodePtr = firstNodePtr->nextNode ? firstNodePtr->nextNode : nullptr;
            _head.nextNode = secondNodePtr;
            delete firstNodePtr;
        }

        _size = 0;
    }

    void swap(SingleLinkedList& rhs) noexcept {
        std::size_t tmpSize = _size;
        _size = rhs._size;
        rhs._size = tmpSize;

        std::swap(_head.nextNode, rhs._head.nextNode);
    }

   private:
    Node _head = Node();  // Фиктивный узел, используется для вставки "перед первым элементом"
    std::size_t _size = 0;
};

// Эта функция тестирует базовую реализацию SingleLinkedList (без итераторв)
void Test1() {
    // Шпион, следящий за своим удалением
    struct DeletionSpy {
        DeletionSpy() = default;
        explicit DeletionSpy(int& instanceCounter) noexcept : _instanceCounterPtr(&instanceCounter) {
            OnAddInstance();
        }

        DeletionSpy(const DeletionSpy& other) noexcept : _instanceCounterPtr(other._instanceCounterPtr) {
            OnAddInstance();
        }

        DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
            if (this != &rhs) {
                auto rhs_copy(rhs);
                std::swap(_instanceCounterPtr, rhs_copy._instanceCounterPtr);
            }

            return *this;
        }

        ~DeletionSpy() {
            OnDeleteInstance();
        }

       private:
        void OnAddInstance() noexcept {
            if (_instanceCounterPtr) {
                ++(*_instanceCounterPtr);
            }
        }

        void OnDeleteInstance() noexcept {
            if (_instanceCounterPtr) {
                assert(*_instanceCounterPtr != 0);
                --(*_instanceCounterPtr);
            }
        }

       private:
        int* _instanceCounterPtr = nullptr;
    };

    // Проверка вставки в начало
    {
        SingleLinkedList<int> l;
        assert(l.IsEmpty());
        assert(l.GetSize() == 0u);

        l.PushFront(0);
        l.PushFront(1);
        assert(l.GetSize() == 2);
        assert(!l.IsEmpty());

        l.Clear();
        assert(l.GetSize() == 0);
        assert(l.IsEmpty());
    }

    // Проверка фактического удаления элементов
    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            SingleLinkedList<DeletionSpy> list;
            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});

            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
            list.Clear();
            assert(item0_counter == 0);
            assert(item1_counter == 0);
            assert(item2_counter == 0);

            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
        }
        assert(item0_counter == 0);  // <== ЗАВАЛИВАЕТСЯ ЗДЕСЬ!!!
        assert(item1_counter == 0);
        assert(item2_counter == 0);
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    {
        bool exception_was_thrown = false;
        // Последовательно уменьшаем счётчик копирований до нуля, пока не будет выброшено исключение
        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            // Создаём непустой список
            SingleLinkedList<ThrowOnCopy> list;
            list.PushFront(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.PushFront(ThrowOnCopy(copy_counter));
                // Если метод не выбросил исключение, список должен перейти в новое состояние
                assert(list.GetSize() == 2);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                // После выбрасывания исключения состояние списка должно остаться прежним
                assert(list.GetSize() == 1);
                break;
            }
        }
        assert(exception_was_thrown);
    }
}

// Эта функция тестирует реализацию SingleLinkedList С ИТЕРАТОРАМИ
void Test2() {
    // Итерирование по пустому списку
    {
        SingleLinkedList<int> list;
        // Константная ссылка для доступа к константным версиям begin()/end()
        const auto& const_list = list;

        // Итераторы begin и end у пустого диапазона равны друг другу
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }

    // Итерирование по непустому списку
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());

        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());

        assert(const_list.begin() == const_list.cbegin());

        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);

        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);

        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        // Проверка прединкремента
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // Проверка постинкремента
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // Итератор, указывающий на позицию после последнего элемента, равен итератору end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    // Преобразование итераторов
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // Конструирование ConstIterator из Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());

        SingleLinkedList<int>::ConstIterator const_it1;
        // Присваивание ConstIterator'у значения Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // Проверка оператора ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
}

// мои тесты
void Test3() {
    // Тестирование НЕКОНСТАНТНОГО и КОНСТАНТНОГО СПИСКА
    {
        SingleLinkedList<int> myList;
        myList.PushFront(1);
        myList.PushFront(2);
        myList.PushFront(3);

        SingleLinkedList<int>::Iterator it = myList.begin();
        // SingleLinkedList<int>::BasicIterator<int> it = myList.begin();  // ТАКАЯ конструкция почему-то не работает

        std::cout << "*it = " << *it << std::endl;

        auto copiedIt = it;
        std::cout << "*copiedIt = " << *copiedIt << std::endl;
    }
}

void Test4() {
    // Тестирование обхода элементов списка и применения к ним STL-алгоритмов
    {
        using namespace std::literals;

        SingleLinkedList<int> myList = {1, 42, 18, 5, 3};

        std::cout << "myList: ";
        PrintRange(myList.begin(), myList.end());

        if (const auto it = std::find(myList.cbegin(), myList.cend(), 42); it != myList.cend()) {
            std::cout << "The list contains 42"s << std::endl;
        } else {
            std::cout << "The list doesn't contain 42"s << std::endl;
        }
    }
}

void Test5() {
    // Тестирование конструктора копирования
    {
        using namespace std::literals;

        SingleLinkedList<int> myList = {1, 42, 18, 5, 3};
        SingleLinkedList<int> myListCopy = myList;
        std::cout << "myList: ";
        PrintRange(myList.begin(), myList.end());
        std::cout << std::endl;
        std::cout << "myListCopy: ";
        PrintRange(myList.begin(), myList.end());
    }
}

int main() {
    // Test3();
    // Test4();
    Test5();
}