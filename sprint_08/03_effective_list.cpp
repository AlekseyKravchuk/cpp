#include <algorithm>
#include <iostream>
#include <iterator>  // std::inserter, std::next
#include <list>

using namespace std;

class Editor {
   public:
    Editor() {
        _cursor = _text.begin();
    }

    // сдвинуть курсор влево
    void Left() {
        if (!_text.empty() && (_cursor != _text.begin())) {
            --_cursor;
        }
    }

    // сдвинуть курсор вправо
    void Right() {
        if (!_text.empty() && (_cursor != _text.end())) {
            _cursor++;
        }
    }

    // вставить символ token
    void Insert(char token) {
        _cursor = _text.insert(_cursor, token);
        ++_cursor;
    }

    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        if (!_text.empty()) {
            Copy(tokens);

            // проверяем, что запрашиваемое количество символов может быть скопировано, начиная с данной позиции курсора
            if (std::distance(_cursor, _text.end()) >= static_cast<int>(tokens)) {
                _cursor = _text.erase(_cursor, std::next(_cursor, tokens));
            } else {
                _cursor = _text.erase(_cursor, _text.end());
            }
        }
    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        _buffer.clear();

        if (tokens) {
            // проверяем, что запрашиваемое количество символов может быть скопировано, начиная с данной позиции курсора
            size_t numOfTokensLeft = std::abs(std::distance(_cursor, _text.end()));
            if (numOfTokensLeft >= tokens) {
                _buffer.assign(_cursor, std::next(_cursor, tokens));

                // another way to copy elements
                // std::copy(_cursor, std::next(_cursor, tokens), std::back_inserter(_buffer));
            } else {
                _buffer.assign(_cursor, _text.end());
            }
        }
    }

    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        if (!_buffer.empty()) {
            _text.insert(_cursor, _buffer.begin(), _buffer.end());
        }
    }

    // получить текущее содержимое текстового редактора
    string GetText() const {
        string resString;
        for (auto ch : _text) {
            resString += ch;
        }

        return resString;
    }

    // ========= Мои проверочные методы =========
    char getPrevCharOfCursor() {
        if (_cursor != _text.begin()) {
            return *(std::prev(_cursor));
        } else {
            return '#';
        }
    }

    char getNextCharOfCursor() {
        if (_cursor != _text.end()) {
            return *(std::next(_cursor));
        } else {
            return '#';
        }
    }
    // ==========================================

   private:
    list<char> _text;
    list<char> _buffer;
    list<char>::iterator _cursor;
};

void MyTests() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    std::cout << "After inserting" << std::endl;

    // передвигаем курсор на символ 'w' слова "world"
    for (int i = 0; i < 5; ++i) {
        editor.Left();
    }

    // должно получиться "hello, aaa_world"
    const string textBeforeWorld = "aaa_";
    for (char c : textBeforeWorld) {
        editor.Insert(c);
    }
    std::cout << editor.GetText() << std::endl;
    std::cout << editor.getPrevCharOfCursor() << std::endl;

    // Тестирование метода Cut()
    editor.Cut(3);
    std::cout << editor.GetText() << std::endl;
    std::cout << editor.getPrevCharOfCursor() << std::endl;

    std::cout << "Finished" << std::endl;
}

int main() {
    Editor editor;
    const string text = "hello, world"s;

    for (char c : text) {
        editor.Insert(c);
    }

    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }

    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);

    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }

    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');

    // Текущее состояние редактора: `world, |`
    editor.Paste();

    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();

    std::cout << std::endl;

    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа

    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();

    std::cout << "previous character of cursor: "s << editor.getPrevCharOfCursor() << std::endl;
    std::cout << "NEXT character of cursor: "s << editor.getNextCharOfCursor() << std::endl;

    return 0;
}