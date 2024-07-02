#include <algorithm>
#include <cassert>
#include <iostream>
#include <new>  // Для исключения bad_alloc
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class PtrVector {
   public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Резервируем место в vector-е для хранения нужного количества элементов
        // Благодаря этому при push_back не будет выбрасываться исключение
        _items.reserve(other._items.size());

        try {
            for (auto p : other._items) {
                // Копируем объект, если указатель на него ненулевой
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение

                // Не выбросит исключение, т. к. в vector память уже зарезервирована
                _items.push_back(p_copy);
            }
        } catch (...) {
            // удаляем элементы в векторе и перевыбрасываем пойманное исключение
            DeleteItems();
            throw;
        }
    }

    PtrVector& operator=(const PtrVector& rhs) {
        if (this != &rhs) {
            // Реализация операции присваивания с помощью идиомы Copy-and-swap.
            // Если исключение будет выброшено, то на текущий объект оно не повлияет.
            // вызываем конструктор копирования
            auto rhsCopy(rhs);

            // rhsCopy содержит копию правого аргумента "rhs".
            // Обмениваемся с ним данными.
            swap(rhsCopy);

            // Теперь текущий объект содержит копию правого аргумента,
            // а rhsCopy - прежнее состояние текущего объекта, которое при выходе
            // из блока будет разрушено.
        }

        return *this;
    }

    // обменивает состояние текущего объекта с other без выбрасывания исключений
    void swap(PtrVector& other) noexcept {
        this->GetItems().swap(other.GetItems());
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        return _items;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        return _items;
    }

   private:
    void DeleteItems() noexcept {
        for (auto p : _items) {
            delete p;
        }
    }

    std::vector<T*> _items;
};

// Щупальце
class Tentacle {
   public:
    explicit Tentacle(int id) noexcept : _id(id) {}

    int GetId() const noexcept { return _id; }

    Tentacle* GetLinkedTentacle() const noexcept {
        return _linkedTentacle;
    }

    void LinkTo(Tentacle& tentacle) noexcept {
        _linkedTentacle = &tentacle;
    }

    void Unlink() noexcept {
        _linkedTentacle = nullptr;
    }

   private:
    int _id = 0;
    Tentacle* _linkedTentacle = nullptr;
};

// Осьминог
class Octopus {
   public:
    Octopus() : Octopus(8) {}

    explicit Octopus(int numTentacles) {
        Tentacle* tentacle = nullptr;
        try {
            for (int i = 1; i <= numTentacles; ++i) {
                tentacle = new Tentacle(i);                 // Может выбросить исключение bad_alloc
                _tentacles.GetItems().push_back(tentacle);  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в _tentacles,
                // чтобы не удалить его в обработчике catch повторно
                tentacle = nullptr;
            }
        } catch (const std::bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер _tentacles
            Cleanup();
            // Удаляем щупальце, которое создали, но не добавили в _tentacles
            delete tentacle;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }
    

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
    Tentacle& AddTentacle() {
        auto lastTentacle = _tentacles.GetItems().back();
        int lastTentacleID = lastTentacle->GetId();
        Tentacle* tentacle = new Tentacle(lastTentacleID + 1);  // Может выбросить исключение bad_alloc
        _tentacles.GetItems().push_back(tentacle);              // Может выбросить исключение bad_alloc

        return *tentacle;
    }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(_tentacles.GetItems().size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *_tentacles.GetItems().at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *_tentacles.GetItems().at(index);
    }

   private:
    void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : _tentacles.GetItems()) {
            delete t;
        }
        // Очищаем массив указателей на щупальца
        _tentacles.GetItems().clear();
    }

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    PtrVector<Tentacle> _tentacles;
};

// Эта функция main тестирует шаблон класса PtrVector
int main() {
    struct CopyingSpy {
        CopyingSpy(int& copy_count, int& deletion_count)
            : copy_count_(copy_count), deletion_count_(deletion_count) {
        }
        CopyingSpy(const CopyingSpy& rhs)
            : copy_count_(rhs.copy_count_)  // счётчик копирований
              ,
              deletion_count_(rhs.deletion_count_)  // счётчик удалений
        {
            if (rhs.throw_on_copy_) {
                throw runtime_error("copy construction failed"s);
            }
            ++copy_count_;
        }
        ~CopyingSpy() {
            ++deletion_count_;
        }
        void ThrowOnCopy() {
            throw_on_copy_ = true;
        }

       private:
        int& copy_count_;
        int& deletion_count_;
        bool throw_on_copy_ = false;
    };

    // Проверка присваивания
    {
        int item0_copy_count = 0;
        int item0_deletion_count = 0;
        {
            PtrVector<CopyingSpy> v;

            v.GetItems().push_back(new CopyingSpy(item0_copy_count, item0_deletion_count));
            v.GetItems().push_back(nullptr);
            {
                PtrVector<CopyingSpy> v_copy;
                v_copy = v;
                assert(v_copy.GetItems().size() == v.GetItems().size());
                assert(v_copy.GetItems().at(0) != v.GetItems().at(0));
                assert(v_copy.GetItems().at(1) == nullptr);
                assert(item0_copy_count == 1);
                assert(item0_deletion_count == 0);
            }
            assert(item0_deletion_count == 1);
        }
        assert(item0_deletion_count == 2);
    }

    // Проверка корректности самоприсваивания
    {
        int item0_copy_count = 0;
        int item0_deletion_count = 0;

        PtrVector<CopyingSpy> v;
        v.GetItems().push_back(new CopyingSpy(item0_copy_count, item0_deletion_count));
        CopyingSpy* first_item = v.GetItems().front();

        v = v;
        assert(v.GetItems().size() == 1);
        // При самоприсваивании объекты должны быть расположены по тем же адресам
        assert(v.GetItems().front() == first_item);
        assert(item0_copy_count == 0);
        assert(item0_deletion_count == 0);
    }

    // Проверка обеспечения строгой гарантии безопасности исключений при присваивании
    {
        int item0_copy_count = 0;
        int item0_deletion_count = 0;

        int item1_copy_count = 0;
        int item1_deletion_count = 0;

        // v хранит 2 элемента
        PtrVector<CopyingSpy> v;
        v.GetItems().push_back(new CopyingSpy(item0_copy_count, item0_deletion_count));
        v.GetItems().push_back(new CopyingSpy(item1_copy_count, item1_deletion_count));

        int other_item0_copy_count = 0;
        int other_item0_deletion_count = 0;
        // other_vector хранит 1 элемент, при копировании которого будет выброшено исключение
        PtrVector<CopyingSpy> other_vector;
        other_vector.GetItems().push_back(new CopyingSpy(other_item0_copy_count, other_item0_deletion_count));
        other_vector.GetItems().front()->ThrowOnCopy();

        // Сохраняем массив указателей
        auto v_items(v.GetItems());

        try {
            v = other_vector;
            // Операция должна выбросить исключение
            assert(false);
        } catch (const runtime_error&) {
        }

        // Элементы массива должны остаться прежними
        assert(v.GetItems() == v_items);
        assert(item0_copy_count == 0);
        assert(item1_copy_count == 0);
        assert(other_item0_copy_count == 0);
    }
}