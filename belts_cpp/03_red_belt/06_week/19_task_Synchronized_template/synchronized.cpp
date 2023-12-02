#include "test_runner.h"

#include <future>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <thread>
#include <vector>

// Реализуйте шаблон Synchronized<T>. Метод "GetAccess" должен возвращать структуру, в которой есть поле "T& value".
template <typename T>
class Synchronized {
  public:
    // T initial = T() - не что иное как объявление параметра по умолчанию.
    // T{} - значит что в случае вызова без параметров конструктора "Synchronaized" в качестве параметра принимается значение
    // по умолчанию шаблонного типа. Если это "int" - то 0 если "std::string" - то пустая строка.
    explicit Synchronized(T initial = T{})
        : _value(std::move(initial)){};

    // Структура "Acess" имеет ссылочное поле - мы обязаны инициализировать его при создании экземпляра структуры.
    struct Access {
        std::lock_guard<std::mutex> guard;
        T& ref_to_value; // <= разделяемые данные
    };

    // метод возвращает структуру "Access", при этом:
    //  - 1-ое поле структуры "Access" инициализируется ССЫЛКОЙ на разделяемые данные (_value, private-член шаблона "Synchronized");
    //  - 2-ое поле структуры "Access" инициализируется 2-ым private-членом шаблона "Synchronized" - МЬЮТЕКСОМ "_mtx", обернутым в "lock_quard"
    Access GetAccess() {
        return Access{std::lock_guard(_mtx), _value};
    }

  private:
    T _value;
    std::mutex _mtx;
};

void TestConcurrentUpdate() {
    Synchronized<std::string> common_string;

    const size_t add_count = 50000;

    auto updater = [&common_string, add_count] {
        for (size_t i = 0; i < add_count; ++i) {
            // auto access = common_string.GetAccess();
            Synchronized<std::string>::Access access = common_string.GetAccess();
            access.ref_to_value += 'a';
        }
    };

    auto f1 = std::async(updater);
    auto f2 = std::async(updater);

    f1.get();
    f2.get();

    ASSERT_EQUAL(common_string.GetAccess().ref_to_value.size(), 2 * add_count);
}

std::vector<int> Consume(Synchronized<std::deque<int>>& common_queue) {
    std::vector<int> got;

    for (;;) {
        std::deque<int> q;

        {
            // Мы специально заключили эти две строчки в операторные скобки, чтобы уменьшить размер критической секции.
            // Поток-потребитель захватывает мьютекс, перемещает всё содержимое общей очереди в свою локальную переменную и отпускает мьютекс.
            // После этого он обрабатывает объекты в очереди за пределами критической секции, позволяя потоку-производителю параллельно
            // помещать в очередь новые объекты.
            //
            // Размер критической секции существенно влияет на быстродействие многопоточных программ.

            // auto access = common_queue.GetAccess();
            Synchronized<std::deque<int>>::Access access = common_queue.GetAccess();
            q = std::move(access.ref_to_value);
        }

        for (int item : q) {
            if (item > 0) {
                got.push_back(item);
            } else {
                return got;
            }
        }
    }
}

void TestProducerConsumer() {
    Synchronized<std::deque<int>> common_queue;

    auto consumer = std::async(Consume, std::ref(common_queue));

    const size_t item_count = 100000;
    for (size_t i = 1; i <= item_count; ++i) {
        common_queue.GetAccess().ref_to_value.push_back(i);
    }

    common_queue.GetAccess().ref_to_value.push_back(-1);

    std::vector<int> expected(item_count);
    std::iota(std::begin(expected), std::end(expected), 1);
    ASSERT_EQUAL(consumer.get(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestProducerConsumer);
}
