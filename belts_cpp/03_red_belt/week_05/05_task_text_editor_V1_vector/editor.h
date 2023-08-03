#include <algorithm>
#include <vector>
#include <iterator>  // std::inserter, std::distance


class Editor {
   public:
    Editor() = default;            // конструктор по умолчанию
    void Left();                   // сдвинуть курсор влево
    void Right();                  // сдвинуть курсор вправо
    void Insert(char token);       // вставить символ token
    void Cut(size_t tokens = 1);   // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1);  // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Paste();                  // вставить содержимое буфера в текущую позицию курсора
    std::string GetText() const;   // получить текущее содержимое текстового редактора

   private:
    uint64_t _cursor_pos{};
    std::vector<char> _text;
    std::vector<char> _cliboard;
};

void TypeText(Editor& editor, const std::string& text);
