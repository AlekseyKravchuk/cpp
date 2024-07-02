#include <iostream>
#include <string>

using namespace std;

// Уведомляет о своём создании и разрушении
class LifetimeLogger {
   public:
    explicit LifetimeLogger(int id)
        : id_(id)  // Сохраняем идентификатор
    {
        cout << "LifetimeLogger "s << id_ << " created"s << endl;
    }

    ~LifetimeLogger() {
        cout << "LifetimeLogger "s << id_ << " destroyed"s << endl;
    }

   private:
    int id_;
};

int main() {
    // Создаём LifetimeLogger в куче, передавая его конструктору параметр 1
    LifetimeLogger* logger1 = new LifetimeLogger(1);

    LifetimeLogger logger2(2);

    cout << "Delete logger 1"s << endl;
    delete logger1;

    cout << "Exit main"s << endl;
}