#include <map>
#include <string>
#include <utility> // std::move

// Возвращает пустую строку, если имя неизвестно
std::string FindNameByYear(const std::map<int, std::string>& names, int year) {
    auto iter_after = names.upper_bound(year);
    std::string name;

    if (iter_after != names.begin()) {
        name = (--iter_after)->second;
    }

    return name;
}

class Person {
  public:
    // Можно ли здесь как-то соптимизировать код, чтобы он мог работать эффективнее для временных объектов,
    // так, чтобы можно было вызвать "ChangeFirstName" от какой-либо временной строки.
    // Сделаем так, чтобы копирование случалось при вызове функции (ушли от передачи строки по константной ссылке),
    // а не при помещении переданной строки в контейнер
    void ChangeFirstName(int year, const std::string first_name) {
        _first_names[year] = std::move(first_name); // а здесь "first_name" перемещаем внутрь словаря
    }

    // Принимаем аргумент по значению, а потом ПЕРЕМЕЩАЕМ его внутрь контейнера.
    // Рассмотрим возможные варианты вызова "ChangeLastName":
    //    ===> 1) вызываем "ChangeLastName" от ВРЕМЕННОГО объекта: MOVE -> MOVE
    //            Этот временный объект проинициализирует строку "last_name" а поскольку объект временный,
    //            то будет вызван MOVE-ctor (случится перемещение). Далее просходит перемещение в контейнер.
    //            НИКАКИХ копирований не случается.
    //    ===> 2) вызываем "ChangeLastName" не от временного объекта: COPY -> MOVE
    //            Невременный объект-параметр функции скопируется в "last_name". А затем случится перемещение этого
    //            объекта внутрь контейнера.
    void ChangeLastName(int year, const std::string last_name) {
        _last_names[year] = std::move(last_name);
    }
    // ИТОГО: приняв параметр метода по значению, мы можем сделать его универсальным и после этого вызывать его
    //        - как от временных объектов и получать 2 ПЕРЕМЕЩЕНИЯ (MOVE, MOVE);
    //        - так и от постоянных объектов и получать КОПИРОВАНИЕ и ПЕРЕМЕЩЕНИЕ (COPY, MOVE).

    std::string GetFullName(int year) {
        const std::string first_name = FindNameByYear(_first_names, year);
        const std::string last_name = FindNameByYear(_last_names, year);

        if (first_name.empty() && last_name.empty()) {
            return "Incognito";
        } else if (first_name.empty()) {
            return last_name + " with unknown first name";
        } else if (last_name.empty()) {
            return first_name + " with unknown last name";
        } else {
            return first_name + " " + last_name;
        }
    }

  private:
    std::map<int, std::string> _first_names;
    std::map<int, std::string> _last_names;
};

// ИТОГИ:
//    - Если мы хотим принять объект в методе или функции и положить его в контейнер, то если мы будем принимать
//      параметр ПО ЗНАЧЕНИЮ и потом оборачивать в "std::move" при складывании в контейнер, то мы получим эффективный
//      код, который будет одинаково эффективен и для принятия временных объектов (будет всего 2 перемещения: MOVE, MOVE), 
//      так и для принятия постоянных объектов (будет одно копирование, и одно перемещение: COPY, MOVE)
//      ПРИМЕЧАНИЕ: это справедливо только в том случае, когда у объекта немного данных на стеке
int main() {
    Person person;

    // хотим научиться вызывать "ChangeFirstName" от года и какой-то ВРЕМЕННОЙ строки
    person.ChangeFirstName(2018, std::string("Alisa"));

    return 0;
}
