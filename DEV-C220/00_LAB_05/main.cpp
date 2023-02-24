#include <algorithm>
#include <iostream>
#include <memory>  // std::unique_ptr, std::make_unique
#include <string>
#include <vector>

using namespace std::literals;

template <typename T>
class MyDeleter {
   private:
    size_t _arr_size{};

   public:
    size_t GetArrSize() const { return _arr_size; }

    MyDeleter(size_t arr_size) : _arr_size(arr_size) {}

    void operator()(T* ptr) { delete[] ptr; }
};

void AddSuffixToEahElmOfUniquePtr(std::string& str, const std::string& suffix) {
    str = str.append(suffix);
    std::cout << "str = "s << str << std::endl;
}

int main() {
    //Задание 1. unique_ptr
    {
        //1.а - обеспечьте корректное выполнение фрагмента
        {
            // // Хранение сырых указателей чревато учечками памяти
            // // Распечатайте все строки
            // std::vector<std::string*> v = {new std::string("aa"), new std::string("bb"), new std::string("bb")};
            // for (auto str : v) {
            //     std::cout << *str << std::endl;
            // }

            // for (auto ptr: v) {
            //     delete ptr;
            //     ptr = nullptr;
            // }
        }  // в случае хранения в векторе сырых указателей на строки по ЭТОЙ закрывающей скобке произойдет утечка памяти
           // ЕСЛИ не обеспечить корректное освобождение памяти

        // 1.b - модифицируйте задание 1.а :
        {
            // //обеспечьте посредством std::unique_ptr:
            // // - эффективное заполнение (вспомните про разные способы формирования std::unique_ptr),
            // // - безопасное хранение указателей на динамически создаваемые объекты std::string,
            // // - манипулирование и освобождение ресурсов

            // // // !!! Инициализация посредством "std::initializer_list" не работает
            // // std::vector<std::unique_ptr<std::string>> v{
            // //     std::make_unique<std::string>("aa"s),
            // //     std::make_unique<std::string>("bb"s),
            // //     std::make_unique<std::string>("сс"s)};

            // std::vector<std::unique_ptr<std::string>> v;
            // v.push_back(std::make_unique<std::string>("aa"s));
            // v.push_back(std::make_unique<std::string>("bb"s));
            // v.push_back(std::make_unique<std::string>("cc"s));

            // for (const auto& smart_ptr: v) {
            //     std::cout << *smart_ptr << std::endl;
            // }
        }

        //1.c - дополните задание 1.b добавьте возможность изменять хранящиеся строки
        //следующим образом (например, добавить указанный суффикс: "AAA" -> "AAA_1")
        {
            // std::vector<std::string> v{"aa"s, "bb"s, "cc"s};
            // size_t n = v.size();

            // std::unique_ptr<std::string[], MyDeleter<std::string>> p(new std::string[n], MyDeleter<std::string>(n));

            // // А хотелось бы еще создавать обертку, не выписывая тип хранимых элементов явно:
            // // auto p(new std::string[n], MyDeleter<std::string>(n));

            // for (int i = 0; i < p.get_deleter().GetArrSize(); ++i) {
            //     p[i] = v[i];
            //     std::cout << p[i] << std::endl;
            // }

            // for (int i = 0; i < p.get_deleter().GetArrSize(); ++i) {
            //     AddSuffixToEahElmOfUniquePtr(p[i], "_1");
            // }

            // std::cout << "\nAfter adding sufixes to each element of wrapper:"s << std::endl;
            // for (int i = 0; i < p.get_deleter().GetArrSize(); ++i) {
            //     std::cout << p[i] << std::endl;
            // }
        }

        //1.d - динамический массив объектов
        //Создайте unique_ptr, который является оберткой для динамического массива с элементами std::string
        //С помощью unique_ptr::operator[] заполните обернутый массив значениями
        //Когда происходит освобождения памяти?
        {
            // не понял задания - всё это я делал в предыдущих примерах
        }

        //1.e - массивы динамических объектов и пользовательская delete-функция (функтор)
        //Задан стековый массив указателей на динамически созданные объекты
        //Создайте unique_ptr для такого массива
        //Реализуйте пользовательскую delete-функцию (функтор) для корректного
        //освобождения памяти
        {
            // std::string* v[] = {new std::string("aa"), new std::string("bb"), new std::string("cc")};
            // size_t n = std::size(v);

            // std::unique_ptr<std::string[], MyDeleter<std::string>> p(new std::string[n], MyDeleter<std::string>(n));

            // // !!! А хотелось бы еще создавать обертку, не выписывая тип хранимых элементов явно:
            // // auto p(new std::string[n], MyDeleter<std::string>(n));

            // for (int i = 0; i < p.get_deleter().GetArrSize(); ++i) {
            //     p[i] = *(v[i]);
            //     std::cout << p[i] << std::endl;
            // }
        }

        //1.f Создайте и заполните вектор, содержащий unique_ptr для указателей на std::string
        //Посредством алгоритма copy() скопируйте элементы вектора в пустой список с элементами того же типа
        //Подсказка: перемещающие итераторы и шаблон std::make_move_iterator
        {
            std::vector<std::unique_ptr<std::string>> v_source;
            v_source.push_back(std::make_unique<std::string>("aaa"s));
            v_source.push_back(std::make_unique<std::string>("bbb"s));
            v_source.push_back(std::make_unique<std::string>("ccc"s));

            std::vector<std::unique_ptr<std::string>> v_dst;
            std::copy(std::make_move_iterator(v_source.begin()),
                      std::make_move_iterator(v_source.end()),
                      std::back_inserter(v_dst));

            for (const auto& p : v_dst) {
                std::cout << *p << std::endl;
            }
        }
    }
    return 0;
}
