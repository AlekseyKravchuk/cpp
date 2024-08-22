#include <iostream>
#include <fstream>
#include <string>

// Определяем структуру Person
struct Person {
    std::string name;
    int age;

    // Метод сериализации (сохранение в файл)
    void serialize(std::ofstream& ofs) const {
        size_t name_length = name.size();
        ofs.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length)); // Сохраняем длину строки
        ofs.write(name.c_str(), name_length);  // Сохраняем строку
        ofs.write(reinterpret_cast<const char*>(&age), sizeof(age));  // Сохраняем возраст
    }

    // Метод десериализации (восстановление из файла)
    void deserialize(std::ifstream& ifs) {
        size_t name_length;
        ifs.read(reinterpret_cast<char*>(&name_length), sizeof(name_length)); // Читаем длину строки
        name.resize(name_length);
        ifs.read(&name[0], name_length);  // Читаем строку
        ifs.read(reinterpret_cast<char*>(&age), sizeof(age));  // Читаем возраст
    }
};

int main() {
    // Создаем объект
    Person p1 = {"John Doe", 30};

    // Сериализуем объект в файл
    std::ofstream ofs("person.dat", std::ios::binary);
    p1.serialize(ofs);
    ofs.close();

    // Десериализуем объект из файла
    Person p2;
    std::ifstream ifs("person.dat", std::ios::binary);
    p2.deserialize(ifs);
    ifs.close();

    // Выводим восстановленный объект
    std::cout << "Name: " << p2.name << ", Age: " << p2.age << std::endl;

    return 0;
}

