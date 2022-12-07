#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
   public:
    explicit Tentacle(int id) : _id(id) {}

    int GetId() const { return _id; }

   private:
    int _id = 0;
};

// Осьминог
class Octopus {
   public:
    Octopus() {
        // <------ Тело конструктора обновлено
        Tentacle* t = nullptr;

        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);      // Может выбросить исключение bad_alloc (ситуация 1)
                _tentacles.push_back(t);  // Может выбросить исключение bad_alloc (ситуация 2)

                // Обнуляем указатель на щупальце, которое уже добавили в _tentacles,
                // чтобы не удалить его в обработчике catch повторно
                t = nullptr;
            }
        } catch (const bad_alloc&) {  // Обработчик исключения учитывает обе ситуации
            // Удаляем сконструированные щупальца, которые успели попасть в контейнер _tentacles
            Cleanup();

            // Удаляем щупальце, которое создали, но не добавили в _tentacles
            // Операции "delete" можно безопасно передавать указатель, равный "nullptr", — в этом случае она ничего не делает.
            delete t;

            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            // throw;
            throw bad_alloc();
        }
        // ------>
    }

    // <------ Добавлен деструктор
    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца, tentacles),
        // которые должны быть удалены при его разрушении.
        // вызов метода "Cleanup" удаляет созданные в динамической памяти щупальца
        Cleanup();
    }
    // ------>

    const Tentacle& GetTentacle(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= _tentacles.size()) {
            throw out_of_range("Invalid tentacle index"s);
        }

        // Чтобы превратить указатель в ссылку, разыменовываем его
        return *_tentacles[index];
    }

   private:
    // <------ Добавлен метод Cleanup
    void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : _tentacles) {
            delete t;
        }

        // Очищаем массив указателей на щупальца
        _tentacles.clear();
    }
    // ------>

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    vector<Tentacle*> _tentacles;
};

// Напишите функцию main, в ней создайте осьминога и несколько его копий
int main() {
    Octopus oct1;
    Octopus oct2 = oct1;
    return 0;
}
