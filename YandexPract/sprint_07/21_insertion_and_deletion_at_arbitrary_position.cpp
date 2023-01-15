#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), nextNode(next) {}

        Type value;
        Node* nextNode = nullptr;
    };

    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) : _node(node) {}

       public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
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

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept : _node(other._node) {}

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
        try {
            for (auto rev_it = std::rbegin(values); rev_it != std::rend(values); ++rev_it) {
                PushFront(*rev_it);
            }
            _size = values.size();

        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            Clear();
            throw;
        }
    }

    // конструктор копирования, реализованный согласно идиоме copy-and-swap
    // 1) Создать внутри конструктора копирования временный односвязный список и последовательно скопировать внутрь него элементы исходного списка.
    //    Если на этом этапе будет выброшено исключение, деструктор временного списка освободит память от его элементов.
    // 2) Когда временный список будет содержать копию исходного списка, останется использовать метод swap и обменять состояние текущего экземпляра класса и временного списка.
    SingleLinkedList(const SingleLinkedList& other) {
        // Сначала надо удостовериться, что текущий список пуст
        assert(_size == 0 && _head.nextNode == nullptr);

        // создаем временный односвязный список
        SingleLinkedList<Type> tmp;

        // копируем в него элементы из other
        try {
            Node* p = &tmp._head;
            for (auto it = other.cbegin(); it != other.cend(); ++it) {
                p->nextNode = new Node(*it, nullptr);
                p = p->nextNode;
                ++tmp._size;
            }

        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            Clear();
            throw;
        }

        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(tmp);
        // Теперь tmp пуст, а текущий список содержит копию элементов other
    }

    // оператор присваивания
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        // 1) Проверяем, не выполняется ли присваивание списка самому себе, сравниваем адреса левого и правого аргументов операции присваивания.
        assert(&_head != &(rhs._head));

        // 2) конструируем временную копию правого аргумента "rhs"
        // Выбрасывание исключения на этом этапе никак не повлияет на состояние текущего объекта (левый аргумент операции присваивания).
        SingleLinkedList<Type> tmp;

        try {
            Node* p = &tmp._head;
            for (auto it = rhs.cbegin(); it != rhs.cend(); ++it) {
                p->nextNode = new Node(*it, nullptr);
                p = p->nextNode;
                ++tmp._size;
            }

        } catch (const std::bad_alloc& e) {
            // std::cerr << e.what() << '\n';
            // Clear();
            throw;
        }

        // 3) Обмениваем содержимое временной копии и текущего объекта. Операция swap исключений не выбрасывает и выполняется быстро.
        //    Временная копия будет содержать предыдущее значение левого аргумента, а текущий экземпляр — копию правого аргумента операции присваивания.
        swap(tmp);
        // При выходе из операции = временный объект будет разрушен, освободив память от предыдущего содержимого списка.

        return *this;
    }

    ~SingleLinkedList() {
        Clear();
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&_head);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&_head));
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&_head);
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* insertedNodePtr = new Node(value, pos._node->nextNode);
        pos._node->nextNode = insertedNodePtr;
        ++_size;

        return Iterator(insertedNodePtr);
    }

    void PopFront() noexcept {
        if (!this->IsEmpty()) {
            Node* nodeToBeDeletedPtr = _head.nextNode;      // указатель на первый элемент списка, который нужно удалить
            _head.nextNode = nodeToBeDeletedPtr->nextNode;  // указатель на 2-ой элемент списка, который станет 1-м после PopFront
            delete nodeToBeDeletedPtr;
            --_size;
        }
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (pos._node->nextNode) {
            // получаем raw-pointer на узел, подлежащий удалению
            Node* nodeToBeDeletedPtr = pos._node->nextNode;

            // получаем адрес следующего узла
            Node* nodeAfterDeletedPtr = nodeToBeDeletedPtr->nextNode;

            // перенастраиваем указатели
            pos._node->nextNode = nodeAfterDeletedPtr;

            // удаляем, освобождаем память в heap'е
            delete nodeToBeDeletedPtr;

            // уменьшаем размер
            --_size;
        }

        return Iterator(pos._node->nextNode);
    }

    // ====================================================================================

    // Обмениваем содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::size_t tmpSize = this->_size;
        this->_size = other._size;
        other._size = tmpSize;

        std::swap(this->_head.nextNode, other._head.nextNode);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
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

   private:
    Node _head = Node();  // Фиктивный узел, используется для вставки "перед первым элементом"
    size_t _size = 0;
};

// ======================= функции, подлежащии реализации =======================

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    } else {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        [](const Type& lhs, const Type& rhs) {
                                            return lhs > rhs;
                                        });
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs == rhs || lhs < rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

// ===============================================================================

// Эта функция проверяет работу класса SingleLinkedList
void Test4() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Проверка PopFront
    {
        SingleLinkedList<int> numbers{3, 14, 15, 92, 6};
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    // Доступ к позиции, предшествующей begin
    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{1, 2, 3, 4};
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());
    }

    // Вставка элемента после указанной позиции
    {  // Вставка в пустой список
        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        // Вставка в непустой список
        {
            SingleLinkedList<int> lst{1, 2, 3};
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
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

    // Проверка обеспечения строгой гарантии безопасности исключений
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{}};
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    // Удаление элементов после указанной позиции
    {
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{DeletionSpy{}, DeletionSpy{}, DeletionSpy{}};
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
}

int main() {
    Test4();
}