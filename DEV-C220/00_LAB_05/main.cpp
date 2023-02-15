#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    //Задание 1. unique_ptr
    {
        //1.а - обеспечьте корректное выполнение фрагмента
        {
            std::vector<std::string*> v = {new std::string("aa"), new std::string("bb"), new std::string("cc")};

            for (auto str : v) {
                std::cout << *str << std::endl;
            }
            //Распечатайте все строки

            //???
        }  //???

        // 1.b - модифицируйте задание 1.а :
        {
            
            //обеспечьте посредством std::unique_ptr:
            //эффективное заполнение (вспомните про разные способы формирования std::unique_ptr),
            //безопасное хранение указателей на динамически создаваемые объекты std::string,
            //манипулирование,
            //и освобождение ресурсов
            //

            {
                //Распечатайте все строки
                //??? Уничтожение динамически созданных объектов?
            }  //???
        }
    }
    return 0;
}
