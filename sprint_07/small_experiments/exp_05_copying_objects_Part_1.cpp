#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id) : _id(id) { }

    int GetId() const {
        return _id;
    }

private:
    int _id = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        Tentacle* tentaclePtr = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                tentaclePtr = new Tentacle(i);      // Может выбросить исключение bad_alloc
                _tentacles.push_back(tentaclePtr);  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в _tentacles,
                // чтобы не удалить его в обработчике catch повторно
                tentaclePtr = nullptr;
            }
        } catch (const bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер _tentacles
            Cleanup();

            // Удаляем щупальце, которое создали, но не добавили в _tentacles
            delete tentaclePtr;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }

    const Tentacle& GetTentacle(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= _tentacles.size()) {
            throw out_of_range("Invalid tentacle index"s);
        }

        // Чтобы превратить указатель в ссылку, разыменовываем его
        return *_tentacles[index];
    }

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

private:
    void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : _tentacles) {
            delete t;
        }
        // Очищаем массив указателей на щупальца
        _tentacles.clear();
    }

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    vector<Tentacle*> _tentacles;
};

int main() {
    {
        Octopus octopus;
        
        // Мы просто хотели ещё одного осьминога
        Octopus octopus1 = octopus;
        // Всё было хорошо и не предвещало беды...
        // ... до этого момента
    }
    cout << "Congratulations. Everything is OK!"s << endl;
}