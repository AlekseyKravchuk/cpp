#include <iostream>
#include <string>
#include <string_view>

// шаблон "отложенная инициализация" (ленивая инициализация, Lazy Initialization)

class File {
  public:
    File(std::string_view fileName) {
        std::cout << "Opening file " << fileName
        << "(call File::File())  // <== presumably open the file long time before we really need to write to it"
        << std::endl;
    }

    ~File() {
        std::cout << "Closing file" << std::endl;
    }

    File(const File&) = delete;            // запрещаем конструктор копирования
    File& operator=(const File&) = delete; // запрещаем оператор присваивания

    File(File&&) = default;                // default move-конструктор
    File& operator=(File&&) = default;     // default перемещающиий оператор присваивания

    void write(std::string_view str) {
        std::cout << "Writing to file: " << str << std::endl;
    }
};

class Config {
  public:
    Config() : _file{"config.txt"} {
        std::cout << "Config object created" << std::endl;
    }

    void addOption(std::string_view name, std::string_view value) {
        _file.write(name);
        _file.write(" = ");
        _file.write(value);
        _file.write("\n");
    }

  private:
    File _file;
};


// ========================= Problem description =========================
/*
The problem with this implementation is that we presumably open the file long time before we really need to write to it.
This may block other processes from manipulating this file, which is an undesirable side effect.
We would instead open the file when the first call to addOption function occurs.
*/
int main() {
    Config config_file;  // 

    std::cout << "Some operations..." << std::endl;

    config_file.addOption("dark_mode", "true");
    config_file.addOption("font", "DejaVu Sans Mono");
}
