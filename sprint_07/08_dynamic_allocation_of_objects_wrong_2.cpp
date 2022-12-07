#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
   public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

   private:
    int id_ = 0;
};

// Осьминог
class Octopus {
   public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        // Если в процессе конструирования щупалец будет выброшено исключение, обработчик catch их все удалит.
        // Так как пойманное в конструкторе исключение не будет выброшено дальше, конструктор завершится успешно,
        // а клиенту достанется осьминог без конечностей.
        // Другими словами, деструктор класса "Octopus" не будет вызван
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
        }
        // --->
    }

    //<--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

   private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}