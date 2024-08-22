#include <iostream>
#include <fstream>
#include <string>

struct User {
    std::string name;
    int age;
};

void serialize(const User& user, std::ofstream& ofs) {
    // Сначала сохраняем длину строки
    size_t name_length = user.name.size();
    ofs.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));

    // Затем сохраняем саму строку имени
    ofs.write(user.name.c_str(), name_length);

    // И, наконец, сохраняем возраст
    ofs.write(reinterpret_cast<const char*>(&user.age), sizeof(user.age));
}

User deserialize(std::ifstream& ifs) {
    User user;

    // Сначала считываем длину строки имени
    size_t name_length;
    ifs.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));

    // Затем считываем саму строку имени
    user.name.resize(name_length);
    ifs.read(&user.name[0], name_length);

    // И, наконец, считываем возраст
    ifs.read(reinterpret_cast<char*>(&user.age), sizeof(user.age));

    return user;
}

int main() {
    User user1 = {"Alice", 30};

    // Сериализация данных пользователя в файл
    std::ofstream ofs("user_data.bin", std::ios::binary);
    serialize(user1, ofs);
    ofs.close();

    // Десериализация данных пользователя из файла
    std::ifstream ifs("user_data.bin", std::ios::binary);
    User user2 = deserialize(ifs);
    ifs.close();

    std::cout << "Name: " << user2.name << ", Age: " << user2.age << std::endl;

    return 0;
}
