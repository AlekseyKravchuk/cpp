#include <iostream>
#include <string>

using namespace std;

// Уведомляет о своём создании и разрушении
class LifetimeLogger {
public:
    explicit LifetimeLogger(int id) : _id(id) {
        cout << "LifetimeLogger "s << _id << " created"s << endl;
    }

    ~LifetimeLogger() {
        cout << "LifetimeLogger "s << _id << " destroyed"s << endl;
    }

private:
    int _id;
};

int main() {
    // Создаём LifetimeLogger в куче, передавая его конструктору параметр 1
    LifetimeLogger* logger1 = new LifetimeLogger(1);

    LifetimeLogger logger2(2);

    cout << "Delete logger 1"s << endl;
    delete logger1;

    cout << "Exit main"s << endl;
}