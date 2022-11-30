#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
   public:
    explicit Tentacle(int id) : _id(id) {}

    int GetId() const {
        return _id;
    }

   private:
    int _id = 0;
};

// Осьминог
class Octopus {
   public:

   // конструктор класса Octopus
    Octopus() {
        Tentacle* tentaclePtr = nullptr;

        // исключение "bad_alloc" может быть выброшено как в операции "new", так и в методе "push_back" класса "std::vector"
        // обработчик исключения учитывает обе ситуации:
        // 1) удаляет сконструированные щупальца, после чего
        // 2) пробрасывает исключение "bad_alloc" дальше, чтобы отменить конструирование осьминога.
        try {
            for (int i = 1; i <= 8; ++i) {
                tentaclePtr = new Tentacle(i);
                _tentacles.push_back(tentaclePtr);

                // обнуляем указатель после добавления адреса щупальца в вектор
                tentaclePtr = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete tentaclePtr;

            throw bad_alloc();
        }

        for (int i = 1; i <= 8; ++i) {
            _tentacles.push_back(new Tentacle(i));
        }
    }

    // деструктор класса Octopus
    // вызов метода Cleanup удаляет созданные в динамической памяти щупальца. 
    ~Octopus() {
        Cleanup();
    }

   private:
    vector<Tentacle*> _tentacles;

    void Cleanup() {
        for (Tentacle* tentaclePtr : _tentacles) {
            delete tentaclePtr;
        }

        // очищаем вектор из указателей на щупальца (vector<Tentacle*>)
        _tentacles.clear();
    }
};

int main() {
    Octopus octopus;
}