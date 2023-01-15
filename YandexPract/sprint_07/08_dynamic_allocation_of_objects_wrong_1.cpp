// Вы ведущий программист в команде, разрабатывающей софт для роботов-осьминогов.
// Один из джуниор-разработчиков прислал вам на ревью код, моделирующий осьминога.
// Взглянув на программу, вы сразу обнаруживаете в ней несколько утечек памяти и отправляете код на доработку.
// Какое исправление должен сделать программист, чтобы устранить найденные вами проблемы?

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
        for (int i = 1; i <= 8; ++i) {
            // Операция new позволяет передать параметры конструктору создаваемого объекта. 
            _tentacles.push_back(new Tentacle(i));
        }
    }

   private:
    vector<Tentacle*> _tentacles;
};

int main() {
    Octopus octopus;
}