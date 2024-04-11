#include "File.h"

File::File(const std::string& filename)
        : _filename(filename) {
    _fp = fopen(filename.c_str(), "w");

    // Если файл открыт неуспешно, то лучшее,
    // что мы можем сделать здесь в конструкторе - это сгенерировать исключение
    if (_fp == nullptr) {
        throw std::runtime_error("Cannot open the file " + _filename);
    }

    // Если исключения не произошло, то объект создан успешно
}

File::~File() {
    // Возвращаем ресурс.
    // Здесь нам нет необходимости проверять, что файл был успешно открыт, потому что если конструктор
    // не отработал успешно, то в деструктор мы не зайдем
    fclose(_fp);
}

const std::string& File::GetFileName() const {
    return _filename;
}

void File::Write(const std::string& line) {
    fputs(line.c_str(), _fp);
}
